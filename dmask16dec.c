// Copyright (c) 2020, 2021 Alexey Tourbin
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

#include "pfor16.h"
#include "platform.h"

static void dmask16dec_tail(uint16_t *v, size_t n, uint v1, uint mask)
{
    if (unlikely(n == 1))
	goto last;
    uint v0;
    uint16_t *last = v + n - 1;
    do {
	v0 = v[0], v0 += v1, v[0] = v0 & mask;
	v1 = v[1], v1 += v0, v[1] = v1 & mask;
	v += 2;
    } while (v < last);
    if (v == last) {
last:	v0 = v[0], v0 += v1, v[0] = v0 & mask;
    }
}

#if defined(__i386__) || defined(__x86_64__) || defined(__SSE2__)
#include <immintrin.h>

#define XMM2ITER(v, xv, xmask)						\
    do {								\
	__m128i x0 = _mm_loadu_si128((void *)(v + 0));			\
	__m128i x1 = _mm_loadu_si128((void *)(v + 8));			\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	x0 = _mm_add_epi16(x0, _mm_slli_si128(x0, 2));			\
	x1 = _mm_add_epi16(x1, _mm_slli_si128(x1, 2));			\
	x0 = _mm_add_epi16(x0, _mm_slli_si128(x0, 4));			\
	x1 = _mm_add_epi16(x1, _mm_slli_si128(x1, 4));			\
	x0 = _mm_add_epi16(x0, _mm_slli_si128(x0, 8));			\
	x1 = _mm_add_epi16(x1, _mm_slli_si128(x1, 8));			\
	xv = _mm_add_epi16(xv, x0);					\
	_mm_storeu_si128((void *)(v + 0), _mm_and_si128(xv, xmask));	\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	xv = _mm_add_epi16(xv, x1);					\
	_mm_storeu_si128((void *)(v + 8), _mm_and_si128(xv, xmask));	\
    } while (0)

#define YMM2ITER(v, xv, xmask)						\
    do {								\
	__m256i y0 = _mm256_loadu_si256((void *)(v + 0));		\
	__m256i y1 = _mm256_loadu_si256((void *)(v + 16));		\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	y0 = _mm256_add_epi16(y0, _mm256_slli_si256(y0, 2));		\
	y1 = _mm256_add_epi16(y1, _mm256_slli_si256(y1, 2));		\
	y0 = _mm256_add_epi16(y0, _mm256_slli_si256(y0, 4));		\
	y1 = _mm256_add_epi16(y1, _mm256_slli_si256(y1, 4));		\
	y0 = _mm256_add_epi16(y0, _mm256_slli_si256(y0, 8));		\
	y1 = _mm256_add_epi16(y1, _mm256_slli_si256(y1, 8));		\
	xv = _mm_add_epi16(xv, _mm256_extracti128_si256(y0, 0));	\
	_mm_storeu_si128((void *)(v + 0), _mm_and_si128(xv, xmask));	\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	xv = _mm_add_epi16(xv, _mm256_extracti128_si256(y0, 1));	\
	_mm_storeu_si128((void *)(v + 8), _mm_and_si128(xv, xmask));	\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	xv = _mm_add_epi16(xv, _mm256_extracti128_si256(y1, 0));	\
	_mm_storeu_si128((void *)(v + 16), _mm_and_si128(xv, xmask));	\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	xv = _mm_add_epi16(xv, _mm256_extracti128_si256(y1, 1));	\
	_mm_storeu_si128((void *)(v + 24), _mm_and_si128(xv, xmask));	\
    } while (0)

#ifndef __SSSE3__
__attribute__((target("ssse3")))
#endif
static void dmask16dec_ssse3(uint16_t *v, size_t n, int mbits)
{
    uint vx = 0;
    uint mask = (1U << mbits) - 1;
    if (likely(n >= 16)) {
	__m128i xv = _mm_set1_epi32(vx);
	__m128i xmask = _mm_set1_epi16(mask);
	uint16_t *vend = v + n;
	uint16_t *last16 = vend - 16;
	do {
	    XMM2ITER(v, xv, xmask);
	    v += 16;
	} while (v <= last16);
	n = vend - v;
	if (unlikely(n == 0))
	    return;
	vx = _mm_extract_epi16(xv, 7);
    }
    dmask16dec_tail(v, n, vx, mask);
}

#ifndef __AVX2__
__attribute__((target("avx2")))
#endif
static void dmask16dec_avx2(uint16_t *v, size_t n, int mbits)
{
    uint vx = 0;
    uint mask = (1U << mbits) - 1;
    if (likely(n >= 16)) {
	__m128i xv = _mm_set1_epi32(vx);
	__m128i xmask = _mm_set1_epi16(mask);
	uint16_t *vend = v + n;
	uint16_t *last16 = vend - 16;
	if (n < 256)
	    do {
		XMM2ITER(v, xv, xmask);
		v += 16;
	    } while (v <= last16);
	else {
	    uint16_t *last32 = vend - 32;
	    do {
		YMM2ITER(v, xv, xmask);
		v += 32;
	    } while (v <= last32);
	    _mm256_zeroupper();
	    if (v <= last16) {
		XMM2ITER(v, xv, xmask);
		v += 16;
	    }
	}
	n = vend - v;
	if (unlikely(n == 0))
	    return;
	vx = _mm_extract_epi16(xv, 7);
    }
    dmask16dec_tail(v, n, vx, mask);
}

#ifndef __SSSE3__
static void dmask16dec_scalar(uint16_t *v, size_t n, int mbits)
{
    dmask16dec_tail(v, n, 0, (1U << mbits) - 1);
}
#endif

static void *dmask16dec_ifunc()
{
    __builtin_cpu_init();
    if (__builtin_cpu_supports("avx2")) {
	// Slow AVX loads on Excavator, slow vzeroupper on KNL.
	if (__builtin_cpu_is("bdver4") || __builtin_cpu_supports("avx512er"))
	    return dmask16dec_ssse3;
	return dmask16dec_avx2;
    }
#ifndef __SSSE3__
    if (!__builtin_cpu_supports("ssse3"))
	return dmask16dec_scalar;
#endif
    return dmask16dec_ssse3;
}

void dmask16dec(uint16_t *v, size_t n, int mbits) __attribute__((ifunc("dmask16dec_ifunc")));

#elif defined(__ARM_NEON) || defined(__aarch64__)
#include <arm_neon.h>

#define XMM2ITER(v, xv, xmask)						\
    do {								\
	uint16x8_t x0 = vld1q_u16(v + 0);				\
	uint16x8_t x1 = vld1q_u16(v + 8);				\
	xv = vreinterpretq_u16_u8(vqtbl1q_u8(				\
		vreinterpretq_u8_u16(xv),				\
		vreinterpretq_u8_u16(vdupq_n_u16(0x0f0e))));		\
	x0 = vaddq_u16(x0, vextq_u16(vdupq_n_u16(0), x0, 8-1));		\
	x1 = vaddq_u16(x1, vextq_u16(vdupq_n_u16(0), x1, 8-1));		\
	x0 = vaddq_u16(x0, vextq_u16(vdupq_n_u16(0), x0, 8-2));		\
	x1 = vaddq_u16(x1, vextq_u16(vdupq_n_u16(0), x1, 8-2));		\
	x0 = vaddq_u16(x0, vextq_u16(vdupq_n_u16(0), x0, 8-4));		\
	x1 = vaddq_u16(x1, vextq_u16(vdupq_n_u16(0), x1, 8-4));		\
	xv = vaddq_u16(xv, x0);						\
	vst1q_u16(v + 0, vandq_u16(xv, xmask));				\
	xv = vreinterpretq_u16_u8(vqtbl1q_u8(				\
		vreinterpretq_u8_u16(xv),				\
		vreinterpretq_u8_u16(vdupq_n_u16(0x0f0e))));		\
	xv = vaddq_u16(xv, x1);						\
	vst1q_u16(v + 8, vandq_u16(xv, xmask));				\
    } while (0)

void dmask16dec(uint16_t *v, size_t n, int mbits)
{
    uint vx = 0;
    uint mask = (1U << mbits) - 1;
    if (likely(n >= 16)) {
	uint16x8_t xv = vdupq_n_u16(vx);
	uint16x8_t xmask = vdupq_n_u16(mask);
	uint16_t *vend = v + n;
	uint16_t *last16 = vend - 16;
	do {
	    XMM2ITER(v, xv, xmask);
	    v += 16;
	} while (v <= last16);
	n = vend - v;
	if (unlikely(n == 0))
	    return;
	vx = xv[7];
    }
    dmask16dec_tail(v, n, vx, mask);
}

#else

void dmask16dec(uint16_t *v, size_t n, int mbits)
{
    dmask16dec_tail(v, n, 0, (1U << mbits) - 1);
}

#endif
