// Copyright (c) 2020 Alexey Tourbin
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#undef NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <t1ha.h>
#include "bitpack16.h"
#include "bitunpack16.h"

#if defined(__i386__) || defined(__x86_64__)
#include <x86intrin.h>
#define rdtsc() __rdtsc()
#elif defined(__aarch64__)
static inline uint64_t rdtsc(void)
{
    uint64_t t;
    asm volatile("mrs %0, cntvct_el0" : "=r"(t));
    return t;
}
#else
#error "rdtsc not supported"
#endif

// Lehmer random number generator
static inline uint32_t rand32(void)
{
#define R32K UINT64_C(6364136223846793005)
    static uint64_t rand32state = R32K;
    uint32_t ret = rand32state >> 32;
    rand32state *= R32K;
    return ret;
}

#define xmalloc malloc

#ifndef __clang__
__attribute__((noipa))
#endif
__attribute__((noinline))
uint64_t bench(void (*pack)(const uint16_t *v, void *p),
	       void (*unpack)(uint16_t *v, const void *p),
	       size_t m, size_t n, uint64_t h)
{
    uint16_t *v = xmalloc(2 * n);
    uint16_t *w = xmalloc(2 * n);
    unsigned char *s = xmalloc(m * n / 8);
    for (size_t i = 0; i < n; i++)
	v[i] = rand32();
    pack(v, s);
    unpack(w, s);
    unsigned mask = Bmask(m);
    for (size_t i = 0; i < n; i++)
	assert((v[i] & mask) == w[i]);
#ifdef VALGRIND
    double nv = (1 << 20) * log2(n);
#else
    double nv = UINT32_MAX * log2(n);
#endif
    size_t ni = nv / n;
    uint64_t t = rdtsc();
    for (size_t i = 0; i < ni; i++)
	unpack(w, s);
    t = rdtsc() - t;
    fprintf(stderr, "unpack16_%zdx%zd: %.3f cycles per integer\n",
	    m, n, t / nv);
    h = t1ha2_atonce(s, m * n / 8, h);
    free(v), free(w), free(s);
    return h;
}

int main()
{
    uint32_t h = 0;
#define BitPack16_func(m, n, X, N) \
    h = bench(bitpack16_##m##x##N, bitunpack16_##m##x##N, m, N, h);
#include "bitpack16-list.h"
    fprintf(stderr, "h=%08x\n", (unsigned) h);
    return 0;
}
