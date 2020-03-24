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

static void delta16dec_tail(uint16_t *v, size_t n, unsigned v1)
{
    if (unlikely(n == 1))
	goto last;
    unsigned v0;
    uint16_t *last = v + n - 1;
    do {
	v0 = v[0], v0 += v1, v[0] = v0;
	v1 = v[1], v1 += v0, v[1] = v1;
	v += 2;
    } while (v < last);
    if (v == last) {
last:	*v += v1;
    }
}

#if defined(__i386__) || defined(__x86_64__) || defined(__SSE2__)
#include <immintrin.h>

#define XMM2ITER(v, xv)							\
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
	_mm_store_si128((void *)(v + 0), xv);				\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	xv = _mm_add_epi16(xv, x1);					\
	_mm_store_si128((void *)(v + 8), xv);				\
    } while (0)

#define YMM2ITER(v, xv)							\
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
	_mm_store_si128((void *)(v + 0), xv);				\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	xv = _mm_add_epi16(xv, _mm256_extracti128_si256(y0, 1));	\
	_mm_store_si128((void *)(v + 8), xv);				\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	xv = _mm_add_epi16(xv, _mm256_extracti128_si256(y1, 0));	\
	_mm_store_si128((void *)(v + 16), xv);				\
	xv = _mm_shuffle_epi8(xv, _mm_set1_epi16(0x0f0e));		\
	xv = _mm_add_epi16(xv, _mm256_extracti128_si256(y1, 1));	\
	_mm_store_si128((void *)(v + 24), xv);				\
    } while (0)

#ifndef __SSSE3__
__attribute__((target("ssse3")))
#endif
static void delta16dec_ssse3(uint16_t *v, size_t n)
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
    delta16dec_tail(v, n, vx);
}

#ifndef __AVX2__
__attribute__((target("avx2")))
#endif
static void delta16dec_avx2(uint16_t *v, size_t n)
{
    unsigned vx = 0;
    if (likely(n >= 16)) {
	__m128i xv = _mm_set1_epi32(vx);
	uint16_t *vend = v + n;
	uint16_t *last16 = vend - 16;
	if (n < 256)
	    do {
		XMM2ITER(v, xv);
		v += 16;
	    } while (v <= last16);
	else {
	    uint16_t *last32 = vend - 32;
	    do {
		YMM2ITER(v, xv);
		v += 32;
	    } while (v <= last32);
	    _mm256_zeroupper();
	    if (v <= last16) {
		XMM2ITER(v, xv);
		v += 16;
	    }
	}
	n = vend - v;
	if (unlikely(n == 0))
	    return;
	vx = _mm_extract_epi16(xv, 7);
    }
    delta16dec_tail(v, n, vx);
}

#ifndef __SSSE3__
static void delta16dec_scalar(uint16_t *v, size_t n)
{
    delta16dec_tail(v, n, 0);
}
#endif

static void *delta16dec_ifunc()
{
    __builtin_cpu_init();
    if (__builtin_cpu_supports("avx2")) {
	// Slow AVX loads on Excavator, slow vzeroupper on KNL.
	if (__builtin_cpu_is("bdver4") || __builtin_cpu_supports("avx512er"))
	    return delta16dec_ssse3;
	return delta16dec_avx2;
    }
#ifndef __SSSE3__
    if (!__builtin_cpu_supports("ssse3"))
	return delta16dec_scalar;
#endif
    return delta16dec_ssse3;
}

void delta16dec(uint16_t *v, size_t n) __attribute__((ifunc("delta16dec_ifunc")));

#elif defined(__ARM_NEON) || defined(__aarch64__)
#include <arm_neon.h>

#define XMM2ITER(v, xv)							\
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
	vst1q_u16(v + 0, xv);						\
	xv = vreinterpretq_u16_u8(vqtbl1q_u8(				\
		vreinterpretq_u8_u16(xv),				\
		vreinterpretq_u8_u16(vdupq_n_u16(0x0f0e))));		\
	xv = vaddq_u16(xv, x1);						\
	vst1q_u16(v + 8, xv);						\
    } while (0)

void delta16dec(uint16_t *v, size_t n)
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
    delta16dec_tail(v, n, vx);
}

#else

void delta16dec(uint16_t *v, size_t n)
{
    delta16dec_tail(v, n, 0);
}

#endif
