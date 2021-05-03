#include "enc.h"

uchar *pfor16enc63(uint16_t *v, size_t n, uchar *out)
{
    return mempcpy(out, v, 2 * n);
}
