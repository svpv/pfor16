#include <assert.h>
#include "bitunpack16.h"

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

const unsigned char *dec64(const unsigned char *src, size_t len, uint16_t *v, size_t n)
{
    uint16_t *last64 = v + n - 64;
    do {
	unsigned bctl = *src++;
	switch (bctl & Mask(6)) {
#define Case64(M)						\
	case 3 * M + 0:						\
	    bitunpack16_##M##x64(v, src);			\
	    src += M * 64 / 8;					\
	    if (bctl & 0x80) {					\
		unsigned i = src[0] & Mask(6);			\
		unsigned x = Bload16le(src + 1);		\
		v[i] = x, src += 3;				\
	    }							\
	    if (bctl & 0x40) {					\
		unsigned pctl = *src++;				\
		unsigned e0 = pctl & 15, e1 = pctl >> 4;	\
		src = patch64(src, v, M, e0, e1);		\
	    }							\
	    v += 64;						\
	    break
#define Case128(M)						\
	case 3 * M + 1:						\
	    bitunpack16_##M##x128(v, src);			\
	    src += M * 128 / 8;					\
	    if (bctl & 0x80) {					\
		unsigned i = src[0] & Mask(7);			\
		unsigned x = Bload16le(src + 1);		\
		v[i] = x, src += 3;				\
	    }							\
	    if (bctl & 0x40) {					\
		unsigned pctl = *src++;				\
		unsigned e0 = pctl & 15, e1 = pctl >> 4;	\
		src = patch128(src, v, M, e0, e1);		\
	    }							\
	    v += 128;						\
	    break
#define Case256(M)						\
	case 3 * M + 2:						\
	    bitunpack16_##M##x256(v, src);			\
	    src += M * 256 / 8;					\
	    if (bctl & 0x80) {					\
		unsigned i = src[0];				\
		unsigned x = Bload16le(src + 1);		\
		v[i] = x, src += 3;				\
	    }							\
	    if (bctl & 0x40) {					\
		unsigned pctl = *src++;				\
		unsigned e0 = pctl & 15, e1 = pctl >> 4;	\
		src = patch256(src, v, M, e0, e1);		\
	    }							\
	    v += 256;						\
	    break
#define Case(M) Case64(M); Case128(M); Case256(M)
	Case(0);
	Case(1);
	Case(2);
	Case(3);
	Case(4);
	Case(5);
	Case(6);
	Case(7);
	Case(8);
	Case(9);
	Case(10);
	Case(11);
	Case(12);
	Case(13);
	Case(14);
	Case(15);
	Case(16);
	default: assert(0);
	}
    } while (v <= last64);
    return src;
}
