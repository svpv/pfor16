#include "enc.h"

static inline unsigned log2i(unsigned x)
{
#ifdef __LZCNT__
    return 31 - __builtin_clz(x);
#endif
    return 31 ^ __builtin_clz(x);
}

// The magnitude of a 16-bit integer: the smallest m such that the integer
// is representable with m bits.
static inline unsigned mag16(uint16_t x)
{
    return log2i(2 * x + 1);
}

// The cumulative histogram of magnitudes for a block of 64 integers:
// h[m] is the number of integers whose magnitude is >= m.
static void histo64(const uint16_t v[64], uint16_t h[32])
{
    memset(h, 0, 32 * 2);
    const uint16_t *v_end = v + 64;
    do {
	h[mag16(v[0])]++;
	h[mag16(v[1])]++;
	h[mag16(v[2])]++;
	h[mag16(v[3])]++;
	v += 4;
    } while (v < v_end);
    uint16_t *hp = h + 16;
    unsigned e = *hp;
    do {
	hp -= 4;
	hp[3] = (e += hp[3]);
	hp[2] = (e += hp[2]);
	hp[1] = (e += hp[1]);
	hp[0] = (e += hp[0]);
    } while (hp > h);
    assert(h[0] == 64);
}

// The cost of decoding a SIMD block (CPU cycles multiplied by 10).
static const uint16_t simd64cost[17] = {
    90, 201, 110, 120, 100, 136, 132, 141, 91, 150, 150, 160, 140, 170, 161, 170, 90
};

static const uint16_t simd128cost[17] = {
    170, 191, 192, 223, 180, 250, 228, 243, 175, 261, 261, 280, 241, 301, 291, 311, 170
};

static const uint16_t simd256cost[17] = {
    330, 351, 351, 392, 330, 451, 438, 466, 348, 492, 471, 609, 532, 550, 532, 591, 330
};

// Branch misprediction penalty.
#define Mispredict 150

// Irreducible costs of decoding each block.
#define BlockTax 200

// The rate-distortion slope: we trade 1-byte reduction in size for 3.2
// extra CPU cycles during decoding.
#define LenRD 32

// Optimal encoding parameters for a block.
struct bp {
    uint8_t m;
    uint8_t e0;
    uint8_t e1;
    uint8_t f;
};

static inline unsigned patchlen(unsigned m, unsigned e0, unsigned e1, unsigned f, unsigned nlog)
{
    unsigned len = e0 * 3 + e1 * 2;
    len += (len > 0); // pctl
    len += f * (3 - (m >= nlog + 4));
    return len;
}

#define patch64len( m, e0, e1, f) patchlen(m, e0, e1, f, 6)
#define patch128len(m, e0, e1, f) patchlen(m, e0, e1, f, 7)
#define patch256len(m, e0, e1, f) patchlen(m, e0, e1, f, 8)

static inline unsigned patchcost(unsigned m, unsigned e0, unsigned e1, unsigned f, unsigned nlog)
{
    unsigned cost = 0;
    if (m >= nlog + 4) { // no e1, odd e0 in f
	cost += Mispredict * 51/100 + f * 25;
	if (e0 == 0)
	    cost += Mispredict * 95/100; // no pctl
	else {
	    cost += Mispredict * 5/100 + 20; // pctl
	    cost += Mispredict + e0 * 54; // patch e0
	}
	return cost;
    }
    if (m < nlog) // a clobber entry, unlikely
	cost += f ? Mispredict * 95/100 + 22 : Mispredict * 5/100;
    if (e0 + e1 == 0)
	cost += Mispredict * 97/100; // no pctl
    else {
	cost += Mispredict * 3/100 + 25; // pctl
	cost += Mispredict + e0 * 54; // patch e0
	cost += Mispredict + e1 * 25; // patch e1
    }
    return cost;
}

#define patch64cost( m, e0, e1, f) patchcost(m, e0, e1, f, 6)
#define patch128cost(m, e0, e1, f) patchcost(m, e0, e1, f, 7)

static inline unsigned patch256cost(unsigned m, unsigned e0, unsigned e1, unsigned f)
{
    unsigned cost = 0;
    if (m >= 12) { // no e1, odd e0 in f
	cost += Mispredict * 51/100 + f * 20;
	if (e0 == 0)
	    cost += Mispredict * 95/100; // no pctl
	else {
	    cost += Mispredict * 5/100 + 20; // pctl
	    cost += Mispredict + e0 * 48; // patch e0
	}
	return cost;
    }
    if (m < 8) // a clobber entry, unlikely
	cost += f ? Mispredict * 95/100 + 20 : Mispredict * 5/100;
    if (e0 + e1 == 0)
	cost += Mispredict * 97/100; // no pctl
    else {
	cost += Mispredict * 3/100 + 30; // pctl
	cost += Mispredict + e0 * 48; // patch e0
	cost += Mispredict + e1 * 20; // patch e1
    }
    return cost;
}

// Find the optimal parameters to encode a block of 64 integers, given
// its cumulative histogram.  Returns RD-weighted TCO.
static unsigned rdopt64(const uint16_t h[32], struct bp *bp)
{
    unsigned m = 16;
    while (h[m] == 0)
	m--;
    // The initial solution: bitpacking without patching.
    unsigned len0 = 1 + m * (64 / 8);
    unsigned cost0 = BlockTax + simd64cost[m];
    unsigned tco0 = LenRD * len0 + cost0;
    bp->m = m;
    bp->e0 = bp->e1 = bp->f = 0;
    // Try smaller m with patching.
    while (1) {
	unsigned e0 = h[m], e1 = h[m+6], f = h[m+10];
	if (f > 1)
	    break;
	e0 -= e1, e1 -= f;
	e1 += e0 & 1, e0 /= 2;
	if (e0 > 15 || e1 > 15)
	    break;
	m--;
	if (m >= 10)
	    f = e1, e1 = 0;
	unsigned len = 1 + m * (64 / 8) + patch64len(m, e0, e1, f);
	unsigned cost = BlockTax + simd64cost[m] + patch64cost(m, e0, e1, f);
	unsigned tco = LenRD * len + cost;
	if (tco0 > tco) {
	    tco0 = tco;
	    len0 = len, cost0 = cost;
	    bp->m = m;
	    bp->e0 = e0, bp->e1 = e1, bp->f = f;
	}
    }
    return tco0;
}

// Likewise, a combined block of 128 integers, given two histograms.
static unsigned rdopt128(const uint16_t h0[32], const uint16_t h1[32], struct bp *bp)
{
    uint16_t h[32];
    memset(h + 16, 0, 16 * 2);
    for (int i = 0; i < 17; i++)
	h[i] = h0[i] + h1[i];
    unsigned m = 16;
    while (h[m] == 0)
	m--;
    // The initial solution: bitpacking without patching.
    unsigned len0 = 1 + m * (128 / 8);
    unsigned cost0 = BlockTax + simd128cost[m];
    unsigned tco0 = LenRD * len0 + cost0;
    bp->m = m;
    bp->e0 = bp->e1 = bp->f = 0;
    // Try smaller m with patching.
    while (1) {
	unsigned e0 = h[m], e1 = h[m+5], f = h[m+9];
	if (f > 1)
	    break;
	e0 -= e1, e1 -= f;
	e1 += e0 & 1, e0 /= 2;
	if (e0 > 15 || e1 > 15)
	    break;
	m--;
	if (m >= 11)
	    f = e1, e1 = 0;
	unsigned len = 1 + m * (128 / 8) + patch128len(m, e0, e1, f);
	unsigned cost = BlockTax + simd128cost[m] + patch128cost(m, e0, e1, f);
	unsigned tco = LenRD * len + cost;
	if (tco0 > tco) {
	    tco0 = tco;
	    len0 = len, cost0 = cost;
	    bp->m = m;
	    bp->e0 = e0, bp->e1 = e1, bp->f = f;
	}
    }
    return tco0;
}

// Likewise, a combined block of 256 integers, given four histograms.
static unsigned rdopt256(const uint16_t h0[32], const uint16_t h1[32],
			 const uint16_t h2[32], const uint16_t h3[32], struct bp *bp)
{
    uint16_t h[32];
    memset(h + 16, 0, 16 * 2);
    for (int i = 0; i < 17; i++)
	h[i] = h0[i] + h1[i] + h2[i] + h3[i];
    unsigned m = 16;
    while (h[m] == 0)
	m--;
    // The initial solution: bitpacking without patching.
    unsigned len0 = 1 + m * (256 / 8);
    unsigned cost0 = BlockTax + simd256cost[m];
    unsigned tco0 = LenRD * len0 + cost0;
    bp->m = m;
    bp->e0 = bp->e1 = bp->f = 0;
    // Try smaller m with patching.
    while (1) {
	unsigned e0 = h[m], e1 = h[m+4], f = h[m+8];
	if (f > 1)
	    break;
	e0 -= e1, e1 -= f;
	e1 += e0 & 1, e0 /= 2;
	if (e0 > 23 || e1 > 31)
	    break;
	m--;
	if (m >= 12)
	    f = e1, e1 = 0;
	unsigned len = 1 + m * (256 / 8) + patch256len(m, e0, e1, f);
	unsigned cost = BlockTax + simd256cost[m] + patch256cost(m, e0, e1, f);
	unsigned tco = LenRD * len + cost;
	if (tco0 > tco) {
	    tco0 = tco;
	    len0 = len, cost0 = cost;
	    bp->m = m;
	    bp->e0 = e0, bp->e1 = e1, bp->f = f;
	}
    }
    return tco0;
}

// We tile the input with blocks of 64, 128 and 256 integers in such a way that
// minimizes the sum of the TCOs.  A tile then describes a block, tentative or
// final, along with the sum of the TCOs up to an including the block.
struct tile {
    uint64_t tco;
    struct bp bp;
    uint8_t span;
};

// Find the optimal tiling by evaluating the blocks of 64 integers and
// their various combinations, using the temporary array [tt,tt+n/64).
// Returns t such that [t,tt+n/64) is the sequence of tiles to be encoded.
static struct tile *enctile(const uint16_t *v, size_t n, struct tile *tt)
{
    // Forward parse: at each iteration, we process the i-th block of 64
    // integers and fill tt[i].  By the end of the iteration, we know the
    // optimal tiling for the input segment up to and including the block.
    // Note that the resulting i-th block implicitly references one of the
    // previous blocks - at (i-1), (i-2), or (i-4), depending on its size.
    uint16_t hh[4][32];
    for (size_t i = 0; i < n / 64; i++) {
	// A block of 64 integers.
	uint16_t *h0 = hh[i%4];
	histo64(v + 64 * i, h0);
	struct tile *t = &tt[i];
	unsigned tco64 = rdopt64(h0, &t->bp);
	uint64_t tco0 = (i > 0) ? t[-1].tco : 0;
	t->tco = tco0 + tco64, t->span = 1;
	// Try a combined block of 128 integers.
	if (i >= 1) {
	    struct bp bp;
	    uint16_t *h1 = hh[(i-1)%4];
	    unsigned tco128 = rdopt128(h0, h1, &bp);
	    tco0 = (i > 1) ? t[-2].tco : 0;
	    if (t->tco > tco0 + tco128) {
		t->tco = tco0 + tco128, t->span = 2;
		t->bp = bp;
	    }
	}
	// Try a combined block of 256 integers.
	if (i >= 3) {
	    struct bp bp;
	    unsigned tco256 = rdopt256(hh[0], hh[1], hh[2], hh[3], &bp);
	    tco0 = (i > 3) ? t[-4].tco : 0;
	    if (t->tco > tco0 + tco256) {
		t->tco = tco0 + tco256, t->span = 4;
		t->bp = bp;
	    }
	}
    }
    // By now we know the optimal tiling for the whole input, but it has to be
    // unraveled from the end.  We follow the back-references and repackage the
    // tiles back to back (clobbering the inactive slots that have been merged
    // into larger blocks).
    intptr_t i = n / 64 - 1;
    struct tile *t = tt + n / 64;
    do {
	*--t = tt[i];
	i -= t->span;
    } while (i >= 0);
    return t;
}

#include "bitpack16.h"

static unsigned ffind(const uint16_t *v, int m)
{
    unsigned i = 0;
    while (1) {
	if (v[i+0] >= (1<<m)) return i+0;
	if (v[i+1] >= (1<<m)) return i+1;
	if (v[i+2] >= (1<<m)) return i+2;
	i += 3;
    }
}

static unsigned char *encpatch(const uint16_t *v, int nlog, int m, unsigned e0, unsigned e1, unsigned char *out)
{
    unsigned char *s0 = out;
    unsigned char *s1 = out + e0 * 3;
    unsigned n0 = 0;
    uint32_t u24 = 0;
    for (unsigned i = 0; i < (1U << nlog); i++) {
	if (v[i] < (1U << m) || v[i] >= (1U << (m + 16 - nlog)))
	    continue;
	if (v[i] >= (1U << (m + 12 - nlog))) {
	    uint16_t w = i | v[i] >> m << nlog;
	    Bstore16le(s1, w);
	    s1 += 2;
	}
	else if ((n0 & 1) == 0) {
	    u24 = i | v[i] >> m << nlog;
	    n0++;
	}
	else {
	    u24 |= i << (24 - nlog);
	    u24 |= v[i] >> m << 12;
	    s0[0] = u24;
	    Bstore16le(s0 + 1, u24 >> 8);
	    s0 += 3;
	    n0++;
	}
    }
    if (n0 & 1) {
	Bstore16le(s1, u24);
	s1 += 2;
	n0--;
    }
    assert(n0 == 2 * e0);
    return s1;
}

static unsigned char *enc64(const uint16_t *v, unsigned char *out, struct tile *t, struct tile *tend)
{
    do {
	if (t->bp.m == 16) {
	    if (t->span == 1) {
		*out++ = 56;
		bitpack16_16x64(v, out);
	    }
	    else if (t->span == 2) {
		*out++ = 57;
		bitpack16_16x128(v, out);
	    }
	    else {
		*out++ = 58;
		bitpack16_16x256(v, out);
	    }
	    v += 64 * t->span;
	    out += 128 * t->span;
	    continue;
	}
	unsigned e0 = t->bp.e0, e1 = t->bp.e1;
	unsigned fflag = t->bp.f, pflag = (e0 + e1) ? 1 : 0;
	switch (log2i(t->span) << 4 | t->bp.m) {
#define CaseC1F3(C, M, N, SpanLog)				\
	case SpanLog << 4 | M:					\
	    *out++ = C << 2 | fflag << 1 | pflag;		\
	    bitpack16_##M##x##N(v, out);			\
	    out += M * N / 8;					\
	    if (fflag) {					\
		unsigned i = ffind(v, M + 10 - SpanLog);	\
		assert(i < N);					\
		*out = i;					\
		Bstore16le(out + 1, v[i]);			\
		out += 3;					\
	    }							\
	    if (pflag) {					\
		*out++ = e0 | e1 << 4;				\
		out = encpatch(v, 6 + SpanLog, M, e0, e1, out); \
	    }							\
	    v += N;						\
	    break
#define CaseC1F0(C, M, N, SpanLog)				\
	case SpanLog << 4 | M:					\
	    *out++ = C << 2 | pflag;				\
	    bitpack16_##M##x##N(v, out);			\
	    out += M * N / 8;					\
	    if (pflag) {					\
		*out++ = e0 | e1 << 4;				\
		out = encpatch(v, 6 + SpanLog, M, e0, e1, out); \
	    }							\
	    v += N;						\
	    break
#define CaseC1F2(C, M, N, SpanLog)				\
	case SpanLog << 4 | M:					\
	    *out++ = C << 2 | fflag << 1 | pflag;		\
	    bitpack16_##M##x##N(v, out);			\
	    out += M * N / 8;					\
	    if (fflag) {					\
		unsigned i = ffind(v, M + 10 - SpanLog);	\
		assert(i < N);					\
		*out = i;					\
		Bstore16le(out + 1, v[i]);			\
		out += 3;					\
	    }							\
	    if (pflag) {					\
		*out++ = e0;					\
		out = encpatch(v, 6 + SpanLog, M, e0, 0, out);	\
	    }							\
	    v += N;						\
	    break

#define CaseC2F3_256(C, M)					\
	case 2 << 4 | M:					\
	    pflag = pflag ? (e0 >> 3) + 1 : pflag;		\
	    *out++ = C << 2 | fflag << 2 | pflag;		\
	    bitpack16_##M##x##256(v, out);			\
	    out += M * 256 / 8;					\
	    if (fflag) {					\
		unsigned i = ffind(v, M + 8);			\
		assert(i < 256);				\
		*out = i;					\
		Bstore16le(out + 1, v[i]);			\
		out += 3;					\
	    }							\
	    if (pflag) {					\
		*out++ = e0 << 5 | e1;				\
		out = encpatch(v, 8, M, e0, e1, out);		\
	    }							\
	    v += 256;						\
	    break
#define CaseC1F0_256(C, M)					\
	case 2 << 4 | M:					\
	    pflag = pflag ? (e0 >> 3) + 1 : pflag;		\
	    *out++ = C << 2 | pflag;				\
	    bitpack16_##M##x##256(v, out);			\
	    out += M * 256 / 8;					\
	    if (pflag) {					\
		*out++ = e0 << 5 | e1;				\
		out = encpatch(v, 8, M, e0, e1, out);		\
	    }							\
	    v += 256;						\
	    break
#define CaseC1F2_256(C, M)					\
	case 2 << 4 | M:					\
	    *out++ = C << 2 | fflag << 1 | pflag;		\
	    bitpack16_##M##x##256(v, out);			\
	    out += M * 256 / 8;					\
	    if (fflag) {					\
		unsigned i = ffind(v, M + 8);			\
		assert(i < 256);				\
		*out = i;					\
		Bstore16le(out + 1, v[i]);			\
		out += 3;					\
	    }							\
	    if (pflag) {					\
		*out++ = e0;					\
		out = encpatch(v, 8, M, e0, 0, out);		\
	    }							\
	    v += 256;						\
	    break

	CaseC1F3( 0,  0, 64, 0); CaseC1F3( 1,  0, 128, 1); CaseC2F3_256( 2,  0);
	CaseC1F3( 4,  1, 64, 0); CaseC1F3( 5,  1, 128, 1); CaseC2F3_256( 6,  1);
	CaseC1F3( 8,  2, 64, 0); CaseC1F3( 9,  2, 128, 1); CaseC2F3_256(10,  2);
	CaseC1F3(12,  3, 64, 0); CaseC1F3(13,  3, 128, 1); CaseC2F3_256(14,  3);
	CaseC1F3(16,  4, 64, 0); CaseC1F3(17,  4, 128, 1); CaseC2F3_256(18,  4);
	CaseC1F3(20,  5, 64, 0); CaseC1F3(21,  5, 128, 1); CaseC2F3_256(22,  5);
	CaseC1F0(24,  6, 64, 0); CaseC1F3(25,  6, 128, 1); CaseC2F3_256(26,  6);
	CaseC1F0(28,  7, 64, 0); CaseC1F0(29,  7, 128, 1); CaseC2F3_256(30,  7);
	CaseC1F0(32,  8, 64, 0); CaseC1F0(33,  8, 128, 1); CaseC1F0_256(34,  8);
	CaseC1F0(35,  9, 64, 0); CaseC1F0(36,  9, 128, 1); CaseC1F0_256(37,  9);
	CaseC1F2(38, 10, 64, 0); CaseC1F0(39, 10, 128, 1); CaseC1F0_256(40, 10);
	CaseC1F2(41, 11, 64, 0); CaseC1F2(42, 11, 128, 1); CaseC1F0_256(43, 11);
	CaseC1F2(44, 12, 64, 0); CaseC1F2(45, 12, 128, 1); CaseC1F2_256(46, 12);
	CaseC1F2(47, 13, 64, 0); CaseC1F2(48, 13, 128, 1); CaseC1F2_256(49, 13);
	CaseC1F2(50, 14, 64, 0); CaseC1F2(51, 14, 128, 1); CaseC1F2_256(52, 14);
	CaseC1F2(53, 15, 64, 0); CaseC1F2(54, 15, 128, 1); CaseC1F2_256(55, 15);

	default:
	    assert(0);
	}
    } while (++t < tend);
    return out;
}

#include <stdlib.h>
#define xmalloc malloc

uchar *pfor16enc64(uint16_t *v, size_t n, uchar *out)
{
    size_t nb = n / 64;
    if (n > (64<<10)) {
	struct tile *tt = xmalloc(nb * sizeof(struct tile));
	struct tile *t = enctile(v, n, tt);
	out = enc64(v, out, t, tt + nb);
	free(tt);
    }
    else {
	struct tile tt[nb];
	struct tile *t = enctile(v, n, tt);
	out = enc64(v, out, t, tt + nb);
    }
    return out;
}
