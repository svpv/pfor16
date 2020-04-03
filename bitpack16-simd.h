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

#pragma once
#include <stdint.h>
#include <string.h>

#if defined(__GNUC__) && __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#define Bswap(k, x) __builtin_bswap##k(x)
#else
#define Bswap(k, x) (x)
#endif

#define Bloadstorefuncs(k)						\
    static inline uint##k##_t Bload##k##le(const void *p)		\
    {									\
	uint##k##_t x;							\
	memcpy(&x, p, k / 8);						\
	return Bswap(k, x);						\
    }									\
    static inline void Bstore##k##le(void *p, uint##k##_t x)		\
    {									\
	x = Bswap(k, x);						\
	memcpy(p, &x, k / 8);						\
    }
Bloadstorefuncs(16)
Bloadstorefuncs(32)
Bloadstorefuncs(64)

#define Baddr8(p, i) ((unsigned char *)(p) + (i))
#define Baddr16(p, i) (void *)((uint16_t *)(p) + (i))
#define Baddr32(p, i) (void *)((uint32_t *)(p) + (i))
#define Baddr64(p, i) (void *)((uint64_t *)(p) + (i))
#define Baddr128(p, i) (void *)((uint64_t *)(p) + 2 * (i))
#define Baddr256(p, i) (void *)((uint64_t *)(p) + 4 * (i))

#define Bmask(k) ((1U << (k)) - 1)

#define Bxextract(B, x, k0, kn, m) B##shr(B##and(x, m), k0)
#define Biextract(B, x, k0, kn, m) B##and(B##shr(x, k0), B##mask(kn))

#define Bxcombine(B, x, y, kx, ky, m)					\
    B##or( B##shr(x, 16 - (kx)),					\
	   B##and(B##shl(y, kx), m) )
#define Bicombine(B, x, y, kx, ky, m)					\
    B##or( B##and(B##shr(x, 16 - (kx)), B##mask(kx)),			\
	   B##shl(B##and(y, B##mask(ky)), kx) )

/*
 * B16x2 pseudo-SIMD engine, backed by uint32_t.
 */
typedef uint32_t B16x2t;
typedef const uint32_t B16x2m;
#define B16x2load(p, i) Bload32le(Baddr32(p, i))
#define B16x2store(p, i, x) Bstore32le(Baddr32(p, i), x)

static inline uint32_t B16x2halfjload(const void *p, size_t i, size_t j)
{
    p = Baddr16(p, 2 * i + j);
    return *Baddr8(p, 0) | *Baddr8(p, 1) << 16;
}

static inline void B16x2halfjstore(const void *p, size_t i, size_t j, uint32_t x)
{
    x |= x >> 8;
    Bstore16le(Baddr16(p, 2 * i + j), x);
}

static inline uint32_t B16x2quarterjload(const void *p, size_t i, size_t j)
{
    uint32_t x = *Baddr8(p, 4 * i + j);
    return (x & 0x0f) | (x & 0x00f0) << 12;
}

static inline void B16x2quarterjstore(const void *p, size_t i, size_t j, uint32_t x)
{
    x |= x >> 12;
    *Baddr8(p, 4 * i + j) = x;
}

#define B16x2halfload(p, i) B16x2halfjload(p, i, 0)
#define B16x2quarterload(p, i) B16x2quarterjload(p, i, 0)
#define B16x2halfstore(p, i, x) B16x2halfjstore(p, i, 0, x)
#define B16x2quarterstore(p, i, x) B16x2quarterjstore(p, i, 0, x)

#define B16x2shl(x, k) ((x) << (k))
#define B16x2shr(x, k) ((x) >> (k))
#define B16x2and(x, m) ((x) & (m))
#define B16x2or(x, y) ((x) | (y))
#define B16x2mask(k) (Bmask(k) | Bmask(k) << 16)
#define B16x2clean(x, m) B16x2and(x, m)
#define B16x2extract(x, k0, kn, m) Biextract(B16x2, x, k0, kn, m)
#define B16x2combine(x, y, kx, ky, m) Bicombine(B16x2, x, y, kx, ky, m)

/*
 * B16x4 pseudo-SIMD engine, backed by uint64_t, or by 2x uint32_t on 32-bit
 * architecutres (to avoid 64-bit shifts).
 */
#if defined(__x86_64__) || UINTPTR_MAX > UINT32_MAX
typedef uint64_t B16x4t;
typedef const uint64_t B16x4m;

#define B16x4load(p, i) Bload64le(Baddr64(p, i))
#define B16x4store(p, i, x) Bstore64le(Baddr64(p, i), x)

// Integer-domain byte order: [1324], expanded to [1 2 |3 4 ];
static inline uint64_t B16x4halfjload(const void *p, size_t i, size_t j)
{
    uint64_t x = Bload32le(Baddr32(p, 2 * i + j));
    return (x & 0x00ff00ff) | (x & 0xff00ff00) << 24;
}

static inline void B16x4halfjstore(const void *p, size_t i, size_t j, uint64_t x)
{
    x |= x >> 24;
    Bstore32le(Baddr32(p, 2 * i + j), x);
}

// SIMD byte order: [1234], for use in B16x8.
static inline uint64_t B16x4halfxload(const void *p, size_t i, size_t j)
{
    uint64_t x = Bload32le(Baddr32(p, 2 * i + j));
    x |= x << 16;
    return (x & 0x000000ff000000ff) | ((x << 8) & 0x00ff000000ff0000);
}

static inline void B16x4halfxstore(const void *p, size_t i, size_t j, uint64_t x)
{
    x |= x >> 8;
    x = (uint16_t) x | ((x >> 16) & 0xffff0000);
    Bstore32le(Baddr32(p, 2 * i + j), x);
}

static inline uint64_t B16x4quarterjload(const void *p, size_t i, size_t j)
{
    uint64_t x = Bload16le(Baddr16(p, 4 * i + j));
    x |= x << 12;
    x |= x << 24;
    return x & 0x000f000f000f000f;
}

static inline void B16x4quarterjstore(const void *p, size_t i, size_t j, uint64_t x)
{
    x |= x >> 24;
    x |= x >> 12;
    Bstore16le(Baddr16(p, 4 * i + j), x);
}

#define B16x4shl(x, k) ((x) << (k))
#define B16x4shr(x, k) ((x) >> (k))
#define B16x4and(x, m) ((x) & (m))
#define B16x4or(x, y) ((x) | (y))
#define B16x4mask(k) (B16x2mask(k) | (uint64_t) B16x2mask(k) << 32)

#else
typedef struct { uint32_t lo, hi; } B16x4t;
typedef const uint32_t B16x4m;

static inline B16x4t B16x4load(const void *p, size_t i)
{
    B16x4t x = { B16x2load(p, 2 * i + 0),
		 B16x2load(p, 2 * i + 1) };
    return x;
}

static inline void B16x4store(const void *p, size_t i, B16x4t x)
{
    B16x2store(p, 2 * i + 0, x.lo);
    B16x2store(p, 2 * i + 1, x.hi);
}

static inline B16x4t B16x4halfjload(const void *p, size_t i, size_t j)
{
    uint32_t y = Bload32le(Baddr32(p, 2 * i + j));
    B16x4t x = { y & 0x00ff00ff, (y & 0xff00ff00) >> 8 };
    return x;
}

static inline void B16x4halfjstore(const void *p, size_t i, size_t j, B16x4t x)
{
    uint32_t y = x.lo | x.hi << 8;
    Bstore32le(Baddr32(p, 2 * i + j), y);
}

static inline B16x4t B16x4halfxload(const void *p, size_t i, size_t j)
{
    p = Baddr32(p, 2 * i + j);
    B16x4t x = { *Baddr8(p, 0) | *Baddr8(p, 1) << 16,
		 *Baddr8(p, 2) | *Baddr8(p, 3) << 16 };
    return x;
}

static inline void B16x4halfxstore(const void *p, size_t i, size_t j, B16x4t x)
{
    uint32_t y = (uint16_t)(x.lo | x.lo >> 8) |
		 (uint16_t)(x.hi | x.hi >> 8) << 16;
    Bstore32le(Baddr32(p, 2 * i + j), y);
}

static inline B16x4t B16x4quarterjload(const void *p, size_t i, size_t j)
{
    uint32_t y = Bload16le(Baddr16(p, 4 * i + j));
    y |= y << 12;
    B16x4t x = { y & 0x000f000f, (y >> 8) & 0x000f000f };
    return x;
}

static inline void B16x4quarterjstore(const void *p, size_t i, size_t j, B16x4t x)
{
    uint32_t y = x.lo | x.hi << 8;
    y |= y >> 12;
    Bstore16le(Baddr16(p, 4 * i + j), y);
}

static inline B16x4t B16x4shl(B16x4t x, int k)
{
    B16x4t y = { x.lo << k, x.hi << k };
    return y;
}

static inline B16x4t B16x4shr(B16x4t x, int k)
{
    B16x4t y = { x.lo >> k, x.hi >> k };
    return y;
}

static inline B16x4t B16x4and(B16x4t x, uint32_t m)
{
    B16x4t y = { x.lo & m, x.hi & m };
    return y;
}

static inline B16x4t B16x4or(B16x4t x, B16x4t y)
{
    B16x4t z = { x.lo | y.lo, x.hi | y.hi };
    return z;
}

#define B16x4mask(k) B16x2mask(k)
#endif

#define B16x4halfload(p, i) B16x4halfjload(p, i, 0)
#define B16x4quarterload(p, i) B16x4quarterjload(p, i, 0)
#define B16x4halfstore(p, i, x) B16x4halfjstore(p, i, 0, x)
#define B16x4quarterstore(p, i, x) B16x4quarterjstore(p, i, 0, x)
#define B16x4clean(x, m) B16x4and(x, m)
#define B16x4extract(x, k0, kn, m) Biextract(B16x4, x, k0, kn, m)
#define B16x4combine(x, y, kx, ky, m) Bicombine(B16x4, x, y, kx, ky, m)

/*
 * B16x8 true SIMD engine, normally backed by an XMM register.
 */
#ifdef __SSE2__
#include <smmintrin.h>
typedef __m128i B16x8t;
typedef const __m128i B16x8m;

#define B16x8load(p, i) _mm_loadu_si128(Baddr128(p, i))
#define B16x8store(p, i, x) _mm_storeu_si128(Baddr128(p, i), x)

static inline __m128i Bpackwb(__m128i x)
{
    return _mm_packus_epi16(x, x);
}

#ifdef __SSE4_1__
#define Bunpackbw(x) _mm_cvtepu8_epi16(x)
#else
#define Bunpackbw(x) _mm_unpacklo_epi8(x, _mm_setzero_si128())
#endif

#define B16x8halfjload(p, i, j) Bunpackbw(_mm_loadl_epi64(Baddr64(p, 2*(i)+(j))))
#define B16x8halfjstore(p, i, j, x) _mm_storel_epi64(Baddr64(p, 2*(i)+(j)), Bpackwb(x))

static inline B16x8t B16x8quarterjload(const void *p, size_t i, size_t j)
{
    __m128i x = _mm_cvtsi32_si128(Bload32le(Baddr32(p, 4 * i + j)));
    x = _mm_or_si128(x, _mm_slli_epi64(x, 28));
    x = _mm_or_si128(x, _mm_slli_si128(x, 7));
    return _mm_and_si128(x, _mm_set1_epi16(0x000f));
}

static inline void B16x8quarterjstore(void *p, size_t i, size_t j, B16x8t x)
{
    x = _mm_or_si128(x, _mm_srli_si128(x, 7));
    x = _mm_or_si128(x, _mm_srli_epi64(x, 28));
    return Bstore32le(Baddr32(p, 4 * i + j), _mm_cvtsi128_si32(x));
}

static inline B16x8t B16x8eighthjload(const void *p, size_t i, size_t j)
{
    __m128i x = _mm_cvtsi32_si128(Bload16le(Baddr16(p, 8 * i + j)));
    x = _mm_or_si128(x, _mm_slli_epi32(x, 14));
    x = _mm_or_si128(x, _mm_slli_epi64(x, 28));
    x = _mm_or_si128(x, _mm_slli_si128(x, 7));
    return _mm_and_si128(x, _mm_set1_epi16(0x0003));
}

static inline void B16x8eighthjstore(void *p, size_t i, size_t j, B16x8t x)
{
    x = _mm_or_si128(x, _mm_srli_si128(x, 7));
    x = _mm_or_si128(x, _mm_srli_epi64(x, 28));
    x = _mm_or_si128(x, _mm_srli_epi32(x, 14));
    return Bstore16le(Baddr16(p, 8 * i + j), _mm_cvtsi128_si32(x));
}

#define B16x8shl(x, k) _mm_slli_epi16(x, k)
#define B16x8shr(x, k) _mm_srli_epi16(x, k)
#define B16x8and(x, m) _mm_and_si128(x, m)
#define B16x8or(x, y) _mm_or_si128(x, y)
#define B16x8mask(k) _mm_set1_epi16(Bmask(k))
#define B16x8clean(x, m) (x)
#define B16x8extract(x, k0, kn, m) Bxextract(B16x8, x, k0, kn, m)
#define B16x8combine(x, y, kx, ky, m) Bxcombine(B16x8, x, y, kx, ky, m)

#else
typedef struct { B16x4t lo, hi; } B16x8t;
typedef B16x4m B16x8m;

static inline B16x8t B16x8load(const void *p, size_t i)
{
    B16x8t x = { B16x4load(p, 2 * i + 0),
		 B16x4load(p, 2 * i + 1) };
    return x;
}

static inline void B16x8store(const void *p, size_t i, B16x8t x)
{
    B16x4store(p, 2 * i + 0, x.lo);
    B16x4store(p, 2 * i + 1, x.hi);
}

static inline B16x8t B16x8halfjload(const void *p, size_t i, size_t j)
{
    B16x8t x = { B16x4halfxload(p, 2 * i + j, 0),
		 B16x4halfxload(p, 2 * i + j, 1) };
    return x;
}

static inline void B16x8halfjstore(const void *p, size_t i, size_t j, B16x8t x)
{
    B16x4halfxstore(p, 2 * i + j, 0, x.lo);
    B16x4halfxstore(p, 2 * i + j, 1, x.hi);
}

static inline B16x8t B16x8quarterjload(const void *p, size_t i, size_t j)
{
#ifdef B16x4load
    uint64_t y = Bload32le(Baddr32(p, 4 * i + j));
    uint64_t m = 0x000f000f000f000f;
    y |= y << 28;
    B16x8t x = { y & m, (y >> 8) & m };
#else
    uint32_t y = Bload32le(Baddr32(p, 4 * i + j));
    uint32_t m = 0x000f000f;
    B16x8t x = { { (y >> 0) & m, (y >>  4) & m },
		 { (y >> 8) & m, (y >> 12) & m } };
#endif
    return x;
}

static inline void B16x8quarterjstore(void *p, size_t i, size_t j, B16x8t x)
{
#ifdef B16x4load
    uint64_t y = x.lo | x.hi << 8;
    y |= y >> 28;
#else
    uint32_t y = x.lo.lo << 0 | x.lo.hi << 4
	       | x.hi.lo << 8 | x.hi.hi << 12;
#endif
    Bstore32le(Baddr32(p, 4 * i + j), y);
}

static inline B16x8t B16x8eighthjload(const void *p, size_t i, size_t j)
{
#ifdef B16x4load
    uint64_t y = Bload16le(Baddr16(p, 8 * i + j));
    uint64_t m = 0x0003000300030003;
    y |= y << 14;
    y |= y << 28;
    B16x8t x = { y & m, (y >> 8) & m };
#else
    uint32_t y = Bload16le(Baddr16(p, 8 * i + j));
    uint32_t m = 0x00030003;
    y |= y << 14;
    B16x8t x = { { (y >> 0) & m, (y >>  4) & m },
		 { (y >> 8) & m, (y >> 12) & m } };
#endif
    return x;
}

static inline void B16x8eighthjstore(void *p, size_t i, size_t j, B16x8t x)
{
#ifdef B16x4load
    uint64_t y = x.lo | x.hi << 8;
    y |= y >> 28;
#else
    uint32_t y = x.lo.lo << 0 | x.lo.hi << 4
	       | x.hi.lo << 8 | x.hi.hi << 12;
#endif
    y |= y >> 14;
    Bstore16le(Baddr32(p, 4 * i + j), y);
}

static inline B16x8t B16x8shl(B16x8t x, int k)
{
    B16x8t y = { B16x4shl(x.lo, k), B16x4shl(x.hi, k) };
    return y;
}

static inline B16x8t B16x8shr(B16x8t x, int k)
{
    B16x8t y = { B16x4shr(x.lo, k), B16x4shr(x.hi, k) };
    return y;
}

static inline B16x8t B16x8and(B16x8t x, B16x8m m)
{
    B16x8t y = { B16x4and(x.lo, m), B16x4and(x.hi, m) };
    return y;
}

static inline B16x8t B16x8or(B16x8t x, B16x8t y)
{
    B16x8t z = { B16x4or(x.lo, y.lo), B16x4or(x.hi, y.hi) };
    return z;
}

#define B16x8mask(k) B16x4mask(k)
#define B16x8clean(x, m) B16x8and(x, m)
#define B16x8extract(x, k0, kn, m) Biextract(B16x8, x, k0, kn, m)
#define B16x8combine(x, y, kx, ky, m) Bicombine(B16x8, x, y, kx, ky, m)
#endif

#define B16x8halfload(p, i) B16x8halfjload(p, i, 0)
#define B16x8quarterload(p, i) B16x8quarterjload(p, i, 0)
#define B16x8eighthload(p, i) B16x8eighthjload(p, i, 0)
#define B16x8halfstore(p, i, x) B16x8halfjstore(p, i, 0, x)
#define B16x8quarterstore(p, i, x) B16x8quarterjstore(p, i, 0, x)
#define B16x8eighthstore(p, i, x) B16x8eighthjstore(p, i, 0, x)

/*
 * B16x16 true SIMD engine, backed by either a YMM register or two B16x8.
 */
#ifdef __AVX2__
#include <immintrin.h>
typedef __m256i B16x16t;
typedef const __m256i B16x16m;

#define B16x16load(p, i) _mm256_loadu_si256(Baddr256(p, i))
#define B16x16store(p, i, x) _mm256_storeu_si256(Baddr256(p, i), x)

#define B16x16halfload(p, i) _mm256_cvtepu8_epi16(_mm_loadu_si128(Baddr256(p, i)))

static inline void B16x16halfstore(const void *p, size_t i, __m256i x)
{
    x = _mm256_packus_epi16(x, x);
    _mm_storel_epi64(Baddr64(p, 4 * i + 0), _mm256_castsi256_si128(x));
    _mm_storel_epi64(Baddr64(p, 4 * i + 1), _mm256_extractf128_si256(x, 1));
}

#define B16x16shl(x, k) _mm256_slli_epi16(x, k)
#define B16x16shr(x, k) _mm256_srli_epi16(x, k)
#define B16x16and(x, m) _mm256_and_si256(x, m)
#define B16x16or(x, y) _mm256_or_si256(x, y)
#define B16x16mask(k) _mm256_set1_epi16(Bmask(k))
#define B16x16clean(x, m) (x)
#define B16x16extract(x, k0, kn, m) Bxextract(B16x16, x, k0, kn, m)
#define B16x16combine(x, y, kx, ky, m) Bxcombine(B16x16, x, y, kx, ky, m)

#else
typedef struct { B16x8t lo, hi; } B16x16t;
typedef B16x8m B16x16m;

static inline B16x16t B16x16load(const void *p, size_t i)
{
    B16x16t x = { B16x8load(p, 2 * i + 0),
		  B16x8load(p, 2 * i + 1) };
    return x;
}

static inline void B16x16store(const void *p, size_t i, B16x16t x)
{
    B16x8store(p, 2 * i + 0, x.lo);
    B16x8store(p, 2 * i + 1, x.hi);
}

static inline B16x16t B16x16halfload(const void *p, size_t i)
{
    B16x16t x = { B16x8halfjload(p, 2 * i, 0),
		  B16x8halfjload(p, 2 * i, 1) };
    return x;
}

static inline void B16x16halfstore(const void *p, size_t i, B16x16t x)
{
    B16x8halfjstore(p, 2 * i, 0, x.lo);
    B16x8halfjstore(p, 2 * i, 1, x.hi);
}

static inline B16x16t B16x16shl(B16x16t x, int k)
{
    B16x16t y = { B16x8shl(x.lo, k), B16x8shl(x.hi, k) };
    return y;
}

static inline B16x16t B16x16shr(B16x16t x, int k)
{
    B16x16t y = { B16x8shr(x.lo, k), B16x8shr(x.hi, k) };
    return y;
}

static inline B16x16t B16x16and(B16x16t x, B16x16m m)
{
    B16x16t y = { B16x8and(x.lo, m), B16x8and(x.hi, m) };
    return y;
}

static inline B16x16t B16x16or(B16x16t x, B16x16t y)
{
    B16x16t z = { B16x8or(x.lo, y.lo), B16x8or(x.hi, y.hi) };
    return z;
}

#define B16x16mask(k) B16x8mask(k)

#ifdef B16x8load
#define B16x16clean(x, m) (x)
#define B16x16extract(x, k0, kn, m) Bxextract(B16x16, x, k0, kn, m)
#define B16x16combine(x, y, kx, ky, m) Bxcombine(B16x16, x, y, kx, ky, m)
#else
#define B16x16clean(x, m) B16x16and(x, m)
#define B16x16extract(x, k0, kn, m) Biextract(B16x16, x, k0, kn, m)
#define B16x16combine(x, y, kx, ky, m) Bicombine(B16x16, x, y, kx, ky, m)
#endif

#endif
