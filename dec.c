#include <assert.h>
#include "bitunpack16.h"
#include "patch.h"

const unsigned char *dec64(const unsigned char *src, size_t len, uint16_t *v, size_t n)
{
    uint16_t *last64 = v + n - 64;
    do {
	unsigned bctl = *src++;
	switch (bctl & 0x7f) {
#define CASE64(M)						\
	case 0 << 5 | M:					\
	    bitunpack16_##M##x64(v, src);			\
	    src += M * 64 / 8;					\
	    if (bctl & 0x80) {					\
		unsigned pctl = *src++;				\
		patch256(src, pctl & 0x7f, v, M, pctl & 0x80);	\
		src += 2 * (pctl & 0x7f) + 2;			\
	    }							\
	    v += 64;						\
	    break
#define CASE128(M)						\
	case 1 << 5 | M:					\
	    bitunpack16_##M##x128(v, src);			\
	    src += M * 128 / 8;					\
	    if (bctl & 0x80) {					\
		unsigned pctl = *src++;				\
		patch256(src, pctl & 0x7f, v, M, pctl & 0x80);	\
		src += 2 * (pctl & 0x7f) + 2;			\
	    }							\
	    v += 128;						\
	    break
#define CASE256(M)						\
	case 2 << 5 | M:					\
	    bitunpack16_##M##x256(v, src);			\
	    src += M * 256 / 8;					\
	    if (bctl & 0x80) {					\
		unsigned pctl = *src++;				\
		patch256(src, pctl & 0x7f, v, M, pctl & 0x80);	\
		src += 2 * (pctl & 0x7f) + 2;			\
	    }							\
	    v += 256;						\
	    break
#define CASE(M) CASE64(M); CASE128(M); CASE256(M)
	CASE(0);
	CASE(1);
	CASE(2);
	CASE(3);
	CASE(4);
	CASE(5);
	CASE(6);
	CASE(7);
	CASE(8);
	CASE(9);
	CASE(10);
	CASE(11);
	CASE(12);
	CASE(13);
	CASE(14);
	CASE(15);
	CASE(16);
	default: assert(0);
	}
    } while (v <= last64);
    return src;
}
