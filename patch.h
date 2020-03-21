#include <stdbool.h>
#include <stdint.h>

#define unlikely(x) __builtin_expect(x, 0)

// Patch a block of 256 m-bit integers by applying n corrections from s,
// in the form of <index, extra high bits> tuples.
static inline bool patch256(const uint8_t *s, intptr_t n, uint16_t *v, int m)
{
    if (--n <= 0) {
	if (unlikely(n < 0))
	    return false;
	goto last;
    }
    const uint8_t *last = s + 2 * n;
    unsigned i0, x0, i1, x1;
    do {
	i0 = s[0];
	x0 = s[1];
	i1 = s[2];
	x1 = s[3];
	s += 4;
	v[i0] |= x0 << m;
	v[i1] |= x1 << m;
    } while (s < last);
    if (s == last) {
last:	i0 = s[0];
	x0 = s[1];
	v[i0] |= x0 << m;
    }
    return true;
}
