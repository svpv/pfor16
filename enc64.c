#include <stdint.h>
#include <string.h>
#include <assert.h>

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
#define Mispredict 66

// The size of a patch body, given e exceptions, f of them fused.
static inline unsigned patchlen(unsigned e, unsigned f)
{
    return 2 * e + 2 * f;
}

// The decoding cost of a patch.
static inline unsigned patchcost(unsigned e, unsigned f)
{
    if (f)
	return 23 * (e - f) + (20 + Mispredict) * f;
    return 21 * e;
}

// The rate-distortion slope: we trade 1-byte reduction in size for 3.2
// extra CPU cycles during decoding.
#define LenRD 32

// Irreducible costs of decoding each block and apply each patch.
#define BlockTax 20
#define PatchTax (30 + Mispredict)

// Optimal encoding parameters for a block.
struct bp {
    uint8_t m;
    uint8_t e;
    uint8_t f;
};

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
    bp->e = bp->f = 0;
    // Try smaller m with patching.
    while (m) {
	unsigned e = h[m], f = h[m+8];
	m--;
	unsigned len = 1 + m * (64 / 8) + 1 + patchlen(e, f);
	unsigned cost = BlockTax + simd64cost[m] + PatchTax + patchcost(e, f);
	unsigned tco = LenRD * len + cost;
	if (tco0 > tco) {
	    tco0 = tco;
	    len0 = len, cost0 = cost;
	    bp->m = m;
	    bp->e = e, bp->f = f;
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
    bp->e = bp->f = 0;
    // Try smaller m with patching.
    while (m) {
	unsigned e = h[m], f = h[m+8];
	m--;
	unsigned len = 1 + m * (128 / 8) + 1 + patchlen(e, f);
	unsigned cost = BlockTax + simd128cost[m] + PatchTax + patchcost(e, f);
	unsigned tco = LenRD * len + cost;
	if (tco0 > tco) {
	    tco0 = tco;
	    len0 = len, cost0 = cost;
	    bp->m = m;
	    bp->e = e, bp->f = f;
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
    bp->e = bp->f = 0;
    // Try smaller m with patching.
    while (m) {
	unsigned e = h[m], f = h[m+8];
	m--;
	unsigned len = 1 + m * (256 / 8) + 1 + patchlen(e, f);
	unsigned cost = BlockTax + simd256cost[m] + PatchTax + patchcost(e, f);
	unsigned tco = LenRD * len + cost;
	if (tco0 > tco) {
	    tco0 = tco;
	    len0 = len, cost0 = cost;
	    bp->m = m;
	    bp->e = e, bp->f = f;
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
