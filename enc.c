#include "enc.h"

size_t pfor16enc(uint16_t *v, size_t n, void *out0)
{
    uchar *out = out0;
    if (n >= 64) {
	out = pfor16enc64(v, n, out);
	v += n & ~63;
	n &= 63;
    }
    if (likely(n > 1))
	out = pfor16enc63(v, n, out);
    else if (n == 1) {
	store16le(out, *v);
	out += 2;
    }
    return out - (const uchar *) out0;
}
