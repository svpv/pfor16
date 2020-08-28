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

#include <stddef.h>
#include <stdint.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(x, 0)

static void delta16enc_tail(uint16_t *v, size_t n, unsigned v1)
{
    if (unlikely(n == 1))
	goto last;
    unsigned v0;
    uint16_t *last = v + n - 1;
    do {
	v0 = v[0], v[0] = v0 - v1;
	v1 = v[1], v[1] = v1 - v0;
	v += 2;
    } while (v < last);
    if (v == last) {
last:	*v -= v1;
    }
}

#if defined(__i386__) || defined(__x86_64__) || defined(__SSE2__)
#include <tmmintrin.h>

#define XMM2ITER(v, xv)							\
    do {								\
	__m128i xw, d;							\
	xw = _mm_loadu_si128((void *)(v + 0));				\
	d = _mm_sub_epi16(xw, _mm_alignr_epi8(xw, xv, 14));		\
	_mm_storeu_si128((void *)(v + 0), d);				\
	xv = _mm_loadu_si128((void *)(v + 8));				\
	d = _mm_sub_epi16(xv, _mm_alignr_epi8(xv, xw, 14));		\
	_mm_storeu_si128((void *)(v + 8), d);				\
    } while (0)

#ifndef __SSSE3__
__attribute__((target("ssse3")))
#endif
static void delta16enc_ssse3(uint16_t *v, size_t n)
{
    unsigned vx = 0;
    if (likely(n >= 16)) {
	__m128i xv = _mm_set1_epi32(vx);
	uint16_t *vend = v + n;
	uint16_t *last16 = vend - 16;
	do {
	    XMM2ITER(v, xv);
	    v += 16;
	} while (v <= last16);
	n = vend - v;
	if (unlikely(n == 0))
	    return;
	vx = _mm_extract_epi16(xv, 7);
    }
    delta16enc_tail(v, n, vx);
}

static void delta16enc_scalar(uint16_t *v, size_t n)
{
    delta16enc_tail(v, n, 0);
}

static void *delta16enc_ifunc()
{
    __builtin_cpu_init();
    // Slow palignr on Bobcat.
    if (__builtin_cpu_supports("ssse3") && !__builtin_cpu_is("btver1"))
	return delta16enc_ssse3;
    return delta16enc_scalar;
}

void delta16enc(uint16_t *v, size_t n) __attribute__((ifunc("delta16enc_ifunc")));

#elif defined(__ARM_NEON) || defined(__aarch64__)
#include <arm_neon.h>

#define XMM2ITER(v, xv)							\
    do {								\
	uint16x8_t xw, d;						\
	xw = vld1q_u16(v + 0);						\
	d = vsubq_u16(xw, vextq_u16(xv, xw, 7));			\
	vst1q_u16(v + 0, d);						\
	xv = vld1q_u16(v + 8);						\
	d = vsubq_u16(xv, vextq_u16(xw, xv, 7));			\
	vst1q_u16(v + 8, d);						\
    } while (0)

void delta16enc(uint16_t *v, size_t n)
{
    unsigned vx = 0;
    if (likely(n >= 16)) {
	uint16x8_t xv = vdupq_n_u16(vx);
	uint16_t *vend = v + n;
	uint16_t *last16 = vend - 16;
	do {
	    XMM2ITER(v, xv);
	    v += 16;
	} while (v <= last16);
	n = vend - v;
	if (unlikely(n == 0))
	    return;
	vx = xv[7];
    }
    delta16enc_tail(v, n, vx);
}

#else

void delta16enc(uint16_t *v, size_t n)
{
    delta16enc_tail(v, n, 0);
}

#endif
