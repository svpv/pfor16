#include <assert.h>
#include "bitunpack16.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(x, 0)

#define Mask(k) ((1U<<(k))-1)

static inline const unsigned char *patch64(const unsigned char *src, uint16_t *v,
	int m, unsigned e0, unsigned e1)
{
    while (e0--) {
	unsigned w0 = Bload16le(src + 0);
	unsigned w1 = Bload16le(src + 1);
	unsigned i0 = w0 & Mask(6);
	unsigned i1 = w1 >> 10;
	unsigned x0 = (w0 >> 6) & Mask(6);
	unsigned x1 = (w1 >> 4) & Mask(6);
	src += 3;
	v[i0] += x0 << m;
	v[i1] += x1 << m;
    }
    while (e1--) {
	unsigned w = Bload16le(src);
	unsigned i = w & Mask(6);
	unsigned x = w >> 6;
	src += 2;
	v[i] += x << m;
    }
    return src;
}

static inline const unsigned char *patch128(const unsigned char *src, uint16_t *v,
	int m, unsigned e0, unsigned e1)
{
    while (e0--) {
	unsigned w0 = Bload16le(src + 0);
	unsigned w1 = Bload16le(src + 1);
	unsigned i0 = w0 & Mask(7);
	unsigned i1 = w1 >> 9;
	unsigned x0 = (w0 >> 7) & Mask(5);
	unsigned x1 = (w1 >> 4) & Mask(5);
	src += 3;
	v[i0] += x0 << m;
	v[i1] += x1 << m;
    }
    while (e1--) {
	unsigned w = Bload16le(src);
	unsigned i = w & Mask(7);
	unsigned x = w >> 7;
	src += 2;
	v[i] += x << m;
    }
    return src;
}

static inline const unsigned char *patch256(const unsigned char *src, uint16_t *v,
	int m, unsigned e0, unsigned e1)
{
    while (e0--) {
	unsigned w0 = Bload16le(src + 0);
	unsigned w1 = Bload16le(src + 1);
	unsigned i0 = (uint8_t) w0;
	unsigned i1 = w1 >> 8;
	unsigned x0 = (w0 >> 8) & Mask(4);
	unsigned x1 = (w1 >> 4) & Mask(4);
	src += 3;
	v[i0] += x0 << m;
	v[i1] += x1 << m;
    }
    while (e1--) {
	unsigned i = src[0];
	unsigned x = src[1];
	src += 2;
	v[i] += x << m;
    }
    return src;
}

__attribute__((flatten))
const unsigned char *dec64(const unsigned char *src, size_t len, uint16_t *v, size_t n)
{
    uint16_t *last64 = v + n - 64;
    do {
	unsigned bctl = *src++;
	switch (bctl >> 2) {
	// with 3-byte f tuple, for small m
#define CaseC1F3(C, M, N)					\
	case C:							\
	    bitunpack16_##M##x##N(v, src);			\
	    src += M * N / 8;					\
	    if (unlikely(bctl & 2)) {				\
		unsigned i = src[0] & (N - 1);			\
		unsigned x = Bload16le(src + 1);		\
		v[i] = x, src += 3;				\
	    }							\
	    if (likely(bctl & 1)) {				\
		unsigned pctl = *src++;				\
		unsigned e0 = pctl & 15, e1 = pctl >> 4;	\
		src = patch##N(src, v, M, e0, e1);		\
	    }							\
	    v += N;						\
	    break
	// without f tuple, for medium m
#define CaseC1F0(C, M, N)					\
	case C:							\
	    bitunpack16_##M##x##N(v, src);			\
	    src += M * N / 8;					\
	    if (likely(bctl & 1)) {				\
		unsigned pctl = *src++;				\
		unsigned e0 = pctl & 15, e1 = pctl >> 4;	\
		src = patch##N(src, v, M, e0, e1);		\
	    }							\
	    v += N;						\
	    break
	// with 2-byte f tuple (e1=0), for big m
#define CaseC1F2(C, M, N)					\
	case C:							\
	    bitunpack16_##M##x##N(v, src);			\
	    src += M * N / 8;					\
	    if (bctl & 2) {					\
		unsigned w = Bload16le(src);			\
		unsigned i = src[0] & (N - 1);			\
		unsigned x = w >> (N == 64 ? 6 : 7);		\
		v[i] += (x << M), src += 2;			\
	    }							\
	    if (likely(bctl & 1)) {				\
		unsigned pctl = *src++;				\
		unsigned e0 = pctl & 15;			\
		src = patch##N(src, v, M, e0, 0);		\
	    }							\
	    v += N;						\
	    break
	// extended pctl, with 3-byte f tuple, for N=256 m=0..7, 8 states:
	// f=0 p=00,01,10,11
	// f=1 p=00,01,10,11
#define CaseC2F3_256(C, M)					\
	case C + 0:						\
	case C + 1:						\
	    bitunpack16_##M##x256(v, src);			\
	    src += M * 256 / 8;					\
	    if (unlikely(bctl & 4)) {				\
		unsigned i = src[0];				\
		unsigned x = Bload16le(src + 1);		\
		v[i] = x, src += 3;				\
	    }							\
	    if (likely(bctl & 3)) {				\
		unsigned pex = (bctl & 3) - 1;			\
		unsigned pctl = *src++ | pex << 8;		\
		unsigned e0 = pctl >> 5, e1 = pctl & 31;	\
		src = patch256(src, v, M, e0, e1);		\
	    }							\
	    v += 256;						\
	    break
	// extended pctl, without f tuple, for N=256 m=8..11
#define CaseC1F0_256(C, M)					\
	case C:							\
	    bitunpack16_##M##x256(v, src);			\
	    src += M * 256 / 8;					\
	    if (likely(bctl & 3)) {				\
		unsigned pex = (bctl & 3) - 1;			\
		unsigned pctl = *src++ | pex << 8;		\
		unsigned e0 = pctl >> 5, e1 = pctl & 31;	\
		src = patch256(src, v, M, e0, e1);		\
	    }							\
	    v += 256;						\
	    break
	// with 2-byte f tuple (e1=0), for N=256 m=12..15
#define CaseC1F2_256(C, M)					\
	case C:							\
	    bitunpack16_##M##x256(v, src);			\
	    src += M * 256 / 8;					\
	    if (bctl & 2) {					\
		unsigned i = src[0];				\
		unsigned x = src[1];				\
		v[i] += (x << M), src += 2;			\
	    }							\
	    if (likely(bctl & 1)) {				\
		unsigned pctl = *src++;				\
		unsigned e0 = pctl & 31;			\
		src = patch256(src, v, M, e0, 0);		\
	    }							\
	    v += 256;						\
	    break

	CaseC1F3( 0,  0, 64); CaseC1F3( 1,  0, 128); CaseC2F3_256( 2,  0);
	CaseC1F3( 4,  1, 64); CaseC1F3( 5,  1, 128); CaseC2F3_256( 6,  1);
	CaseC1F3( 8,  2, 64); CaseC1F3( 9,  2, 128); CaseC2F3_256(10,  2);
	CaseC1F3(12,  3, 64); CaseC1F3(13,  3, 128); CaseC2F3_256(14,  3);
	CaseC1F3(16,  4, 64); CaseC1F3(17,  4, 128); CaseC2F3_256(18,  4);
	CaseC1F3(20,  5, 64); CaseC1F3(21,  5, 128); CaseC2F3_256(22,  5);
	CaseC1F0(24,  6, 64); CaseC1F3(25,  6, 128); CaseC2F3_256(26,  6);
	CaseC1F0(28,  7, 64); CaseC1F0(29,  7, 128); CaseC2F3_256(30,  7);
	CaseC1F0(32,  8, 64); CaseC1F0(33,  8, 128); CaseC1F0_256(34,  8);
	CaseC1F0(35,  9, 64); CaseC1F0(36,  9, 128); CaseC1F0_256(37,  9);
	CaseC1F2(38, 10, 64); CaseC1F0(39, 10, 128); CaseC1F0_256(40, 10);
	CaseC1F2(41, 11, 64); CaseC1F2(42, 11, 128); CaseC1F0_256(43, 11);
	CaseC1F2(44, 12, 64); CaseC1F2(45, 12, 128); CaseC1F2_256(46, 12);
	CaseC1F2(47, 13, 64); CaseC1F2(48, 13, 128); CaseC1F2_256(49, 13);
	CaseC1F2(50, 14, 64); CaseC1F2(51, 14, 128); CaseC1F2_256(52, 14);
	CaseC1F2(53, 15, 64); CaseC1F2(54, 15, 128); CaseC1F2_256(55, 15);

	case 56:
	    bitunpack16_16x64(v, src);
	    src += 16 * 64 / 8;
	    v += 64;
	    break;
	case 57:
	    bitunpack16_16x128(v, src);
	    src += 16 * 128 / 8;
	    v += 128;
	    break;
	case 58:
	    bitunpack16_16x256(v, src);
	    src += 16 * 256 / 8;
	    v += 256;
	    break;

	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
	    return NULL;
	}
    } while (v <= last64);
    return src;
}
