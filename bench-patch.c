// Copyright (c) 2021 Alexey Tourbin
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>

static inline uint16_t Bload16le(const void *p)
{
    uint16_t x;
    memcpy(&x, p, 2);
    return x;
}

#if defined(__i386__) || defined(__x86_64__)
#include <x86intrin.h>
#define rdtsc() __rdtsc()
#else
static inline uint64_t rdtsc(void)
{
    uint64_t t;
#if defined(__aarch64__)
    asm volatile("mrs %0, cntvct_el0" : "=r"(t));
#elif defined(__powerpc64__)
    asm volatile("mfspr %0, 268" : "=r"(t));
#else
#error "rdtsc not supported"
#endif
    return t;
}
#endif

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

// Lehmer random number generator
static inline uint32_t rand32(void)
{
#define R32K UINT64_C(6364136223846793005)
    static uint64_t rand32state = R32K;
    uint32_t ret = rand32state >> 32;
    rand32state *= R32K;
    return ret;
}

#ifndef M
#define M 5
#endif

#ifndef Patch
#define Patch patch256
#endif

#ifndef __clang__
__attribute__((noipa))
#endif
__attribute__((noinline))
void bench(void)
{
    uint16_t v[256];
    unsigned char src[32];
    for (int i = 0; i < 256; i++)
	v[i] = rand32();
    for (int i = 0; i < 32; i++)
	src[i] = rand32();

    uint64_t t[2][32];
    uint32_t n[2][32];
    memset(t, 0, sizeof t);
    memset(n, 0, sizeof n);

    for (int i = 0; i < (1<<28); i++) {
	uint32_t e0 = rand32() % 32;
	uint32_t e1 = rand32() % 32;
	uint64_t t0 = rdtsc();
	Patch(src, v, M, e0, 0);
	uint64_t t1 = rdtsc();
	Patch(src, v, M, 0, e1);
	uint64_t t2 = rdtsc();
	t[0][e0] += t1 - t0; n[0][e0]++;
	t[1][e1] += t2 - t1; n[1][e1]++;
    }
    for (int i = 0; i < 32; i++)
	fprintf(stderr, "e0=%d %.1f cycles\n", i, (double) t[0][i] / n[0][i]);
    for (int i = 0; i < 32; i++)
	fprintf(stderr, "e1=%d %.1f cycles\n", i, (double) t[1][i] / n[1][i]);
}

int main()
{
    bench();
    return 0;
}
