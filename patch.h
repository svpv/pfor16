#include <stdbool.h>
#include <stdint.h>

#define unlikely(x) __builtin_expect(x, 0)

// Patch a block of 256 m-bit integers by applying n corrections from s,
// in the form of <index, extra high bits> tuples.
static inline bool patch256(const uint8_t *s, intptr_t n, uint16_t *v, int m, bool fuse)
{
    if (--n <= 0) {
	if (unlikely(n < 0))
	    return false;
	goto last;
    }
    const uint8_t *last = s + 2 * n;
    unsigned i0, x0, i1, x1;
    if (!fuse)
	do {
	    x0 = s[0];
	    x1 = s[1];
	    i0 = s[2];
	    i1 = s[3];
	    s += 4;
	    v[i0] += x0 << m;
	    v[i1] += x1 << m;
	} while (s < last);
    else
	do {
	    i0 = s[0];
	    i1 = s[1];
	    if (unlikely(i0 == i1)) {
		uint16_t x;
		memcpy(&x, s + 2, 2);
		s += 4;
#if defined(__GNUC__) && __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
		x = __builtin_bswap16(x);
#endif
		v[i0] = x;
	    }
	    else {
		x0 = s[2];
		x1 = s[3];
		s += 4;
		v[i0] += x0 << m;
		v[i1] += x1 << m;
	    }
	} while (s < last);
    if (s == last) {
last:	x0 = s[0];
	i0 = s[1];
	v[i0] += x0 << m;
    }
    return true;
}
