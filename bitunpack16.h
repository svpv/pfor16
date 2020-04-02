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
#include "bitpack16-simd.h"

#define BitUnpack16_1x16(B, v, p)					\
    do {								\
	B##t x;								\
	B##m m = B##mask(1);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 1), m));			\
	B##store(v, 2, B##and(B##shr(x, 2), m));			\
	B##store(v, 3, B##and(B##shr(x, 3), m));			\
	B##store(v, 4, B##and(B##shr(x, 4), m));			\
	B##store(v, 5, B##and(B##shr(x, 5), m));			\
	B##store(v, 6, B##and(B##shr(x, 6), m));			\
	B##store(v, 7, B##and(B##shr(x, 7), m));			\
	B##store(v, 8, B##and(B##shr(x, 8), m));			\
	B##store(v, 9, B##and(B##shr(x, 9), m));			\
	B##store(v, 10, B##and(B##shr(x, 10), m));			\
	B##store(v, 11, B##and(B##shr(x, 11), m));			\
	B##store(v, 12, B##and(B##shr(x, 12), m));			\
	B##store(v, 13, B##and(B##shr(x, 13), m));			\
	B##store(v, 14, B##and(B##shr(x, 14), m));			\
	B##store(v, 15, B##clean(B##shr(x, 15), m));			\
    } while (0)

#define BitUnpack16_2x8(B, v, p)					\
    do {								\
	B##t x;								\
	B##m m = B##mask(2);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 2), m));			\
	B##store(v, 2, B##and(B##shr(x, 4), m));			\
	B##store(v, 3, B##and(B##shr(x, 6), m));			\
	B##store(v, 4, B##and(B##shr(x, 8), m));			\
	B##store(v, 5, B##and(B##shr(x, 10), m));			\
	B##store(v, 6, B##and(B##shr(x, 12), m));			\
	B##store(v, 7, B##clean(B##shr(x, 14), m));			\
    } while (0)

#define BitUnpack16_2x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(2);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 2), m));			\
	B##store(v, 2, B##and(B##shr(x, 4), m));			\
	B##store(v, 3, B##and(B##shr(x, 6), m));			\
	B##store(v, 4, B##and(B##shr(x, 8), m));			\
	B##store(v, 5, B##and(B##shr(x, 10), m));			\
	B##store(v, 6, B##and(B##shr(x, 12), m));			\
	B##store(v, 7, B##clean(B##shr(x, 14), m));			\
	y = B##load(p, 1);						\
	B##store(v, 8, B##and(y, m));					\
	B##store(v, 9, B##and(B##shr(y, 2), m));			\
	B##store(v, 10, B##and(B##shr(y, 4), m));			\
	B##store(v, 11, B##and(B##shr(y, 6), m));			\
	B##store(v, 12, B##and(B##shr(y, 8), m));			\
	B##store(v, 13, B##and(B##shr(y, 10), m));			\
	B##store(v, 14, B##and(B##shr(y, 12), m));			\
	B##store(v, 15, B##clean(B##shr(y, 14), m));			\
    } while (0)

#define BitUnpack16_3x8(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(3);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 3), m));			\
	B##store(v, 2, B##and(B##shr(x, 6), m));			\
	B##store(v, 3, B##and(B##shr(x, 9), m));			\
	B##store(v, 4, B##and(B##shr(x, 12), m));			\
	y = B##halfload(p, 1);						\
	B##store(v, 5, B##combine(x, y, 1, 2, m));			\
	B##store(v, 6, B##and(B##shr(y, 2), m));			\
	B##store(v, 7, B##clean(B##shr(y, 5), m));			\
    } while (0)

#define BitUnpack16_3x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(3);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 3), m));			\
	B##store(v, 2, B##and(B##shr(x, 6), m));			\
	B##store(v, 3, B##and(B##shr(x, 9), m));			\
	B##store(v, 4, B##and(B##shr(x, 12), m));			\
	y = B##load(p, 1);						\
	B##store(v, 5, B##combine(x, y, 1, 2, m));			\
	B##store(v, 6, B##and(B##shr(y, 2), m));			\
	B##store(v, 7, B##and(B##shr(y, 5), m));			\
	B##store(v, 8, B##and(B##shr(y, 8), m));			\
	B##store(v, 9, B##and(B##shr(y, 11), m));			\
	x = B##load(p, 2);						\
	B##store(v, 10, B##combine(y, x, 2, 1, m));			\
	B##store(v, 11, B##and(B##shr(x, 1), m));			\
	B##store(v, 12, B##and(B##shr(x, 4), m));			\
	B##store(v, 13, B##and(B##shr(x, 7), m));			\
	B##store(v, 14, B##and(B##shr(x, 10), m));			\
	B##store(v, 15, B##clean(B##shr(x, 13), m));			\
    } while (0)

#define BitUnpack16_4x4(B, v, p)					\
    do {								\
	B##t x;								\
	B##m m = B##mask(4);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 4), m));			\
	B##store(v, 2, B##and(B##shr(x, 8), m));			\
	B##store(v, 3, B##clean(B##shr(x, 12), m));			\
    } while (0)

#define BitUnpack16_4x8(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(4);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 4), m));			\
	B##store(v, 2, B##and(B##shr(x, 8), m));			\
	B##store(v, 3, B##clean(B##shr(x, 12), m));			\
	y = B##load(p, 1);						\
	B##store(v, 4, B##and(y, m));					\
	B##store(v, 5, B##and(B##shr(y, 4), m));			\
	B##store(v, 6, B##and(B##shr(y, 8), m));			\
	B##store(v, 7, B##clean(B##shr(y, 12), m));			\
    } while (0)

#define BitUnpack16_4x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(4);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 4), m));			\
	B##store(v, 2, B##and(B##shr(x, 8), m));			\
	B##store(v, 3, B##clean(B##shr(x, 12), m));			\
	y = B##load(p, 1);						\
	B##store(v, 4, B##and(y, m));					\
	B##store(v, 5, B##and(B##shr(y, 4), m));			\
	B##store(v, 6, B##and(B##shr(y, 8), m));			\
	B##store(v, 7, B##clean(B##shr(y, 12), m));			\
	x = B##load(p, 2);						\
	B##store(v, 8, B##and(x, m));					\
	B##store(v, 9, B##and(B##shr(x, 4), m));			\
	B##store(v, 10, B##and(B##shr(x, 8), m));			\
	B##store(v, 11, B##clean(B##shr(x, 12), m));			\
	y = B##load(p, 3);						\
	B##store(v, 12, B##and(y, m));					\
	B##store(v, 13, B##and(B##shr(y, 4), m));			\
	B##store(v, 14, B##and(B##shr(y, 8), m));			\
	B##store(v, 15, B##clean(B##shr(y, 12), m));			\
    } while (0)

#define BitUnpack16_5x4(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(5);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 5), m));			\
	B##store(v, 2, B##and(B##shr(x, 10), m));			\
	y = B##quarterload(p, 1);					\
	x = B##clean(B##shr(x, 15), B##mask(1));			\
	B##store(v, 3, B##or(x, B##shl(y, 1)));				\
    } while (0)

#define BitUnpack16_5x8(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(5);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 5), m));			\
	B##store(v, 2, B##and(B##shr(x, 10), m));			\
	y = B##load(p, 1);						\
	B##store(v, 3, B##combine(x, y, 1, 4, m));			\
	B##store(v, 4, B##and(B##shr(y, 4), m));			\
	B##store(v, 5, B##and(B##shr(y, 9), m));			\
	x = B##halfload(p, 2);						\
	B##store(v, 6, B##combine(y, x, 2, 3, m));			\
	B##store(v, 7, B##clean(B##shr(x, 3), m));			\
    } while (0)

#define BitUnpack16_5x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(5);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 5), m));			\
	B##store(v, 2, B##and(B##shr(x, 10), m));			\
	y = B##load(p, 1);						\
	B##store(v, 3, B##combine(x, y, 1, 4, m));			\
	B##store(v, 4, B##and(B##shr(y, 4), m));			\
	B##store(v, 5, B##and(B##shr(y, 9), m));			\
	x = B##load(p, 2);						\
	B##store(v, 6, B##combine(y, x, 2, 3, m));			\
	B##store(v, 7, B##and(B##shr(x, 3), m));			\
	B##store(v, 8, B##and(B##shr(x, 8), m));			\
	y = B##load(p, 3);						\
	B##store(v, 9, B##combine(x, y, 3, 2, m));			\
	B##store(v, 10, B##and(B##shr(y, 2), m));			\
	B##store(v, 11, B##and(B##shr(y, 7), m));			\
	x = B##load(p, 4);						\
	B##store(v, 12, B##combine(y, x, 4, 1, m));			\
	B##store(v, 13, B##and(B##shr(x, 1), m));			\
	B##store(v, 14, B##and(B##shr(x, 6), m));			\
	B##store(v, 15, B##clean(B##shr(x, 11), m));			\
    } while (0)

#define BitUnpack16_6x4(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(6);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 6), m));			\
	y = B##halfload(p, 1);						\
	B##store(v, 2, B##combine(x, y, 4, 2, m));			\
	B##store(v, 3, B##clean(B##shr(y, 2), m));			\
    } while (0)

#define BitUnpack16_6x8(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(6);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 6), m));			\
	y = B##load(p, 1);						\
	B##store(v, 2, B##combine(x, y, 4, 2, m));			\
	B##store(v, 3, B##and(B##shr(y, 2), m));			\
	B##store(v, 4, B##and(B##shr(y, 8), m));			\
	x = B##load(p, 2);						\
	B##store(v, 5, B##combine(y, x, 2, 4, m));			\
	B##store(v, 6, B##and(B##shr(x, 4), m));			\
	B##store(v, 7, B##clean(B##shr(x, 10), m));			\
    } while (0)

#define BitUnpack16_6x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(6);						\
	x = B##load(p, 0);						\
	B##store(v, 0, B##and(x, m));					\
	B##store(v, 1, B##and(B##shr(x, 6), m));			\
	y = B##load(p, 1);						\
	B##store(v, 2, B##combine(x, y, 4, 2, m));			\
	B##store(v, 3, B##and(B##shr(y, 2), m));			\
	B##store(v, 4, B##and(B##shr(y, 8), m));			\
	x = B##load(p, 2);						\
	B##store(v, 5, B##combine(y, x, 2, 4, m));			\
	B##store(v, 6, B##and(B##shr(x, 4), m));			\
	B##store(v, 7, B##clean(B##shr(x, 10), m));			\
	y = B##load(p, 3);						\
	B##store(v, 8, B##and(y, m));					\
	B##store(v, 9, B##and(B##shr(y, 6), m));			\
	x = B##load(p, 4);						\
	B##store(v, 10, B##combine(y, x, 4, 2, m));			\
	B##store(v, 11, B##and(B##shr(x, 2), m));			\
	B##store(v, 12, B##and(B##shr(x, 8), m));			\
	y = B##load(p, 5);						\
	B##store(v, 13, B##combine(x, y, 2, 4, m));			\
	B##store(v, 14, B##and(B##shr(y, 4), m));			\
	B##store(v, 15, B##clean(B##shr(y, 10), m));			\
    } while (0)

#define BitPack16_func(m, n, X, N)					\
    static inline void bitunpack16_##m##x##N(uint16_t *v, const void *p)\
    {									\
	BitUnpack16_##m##x##n(B16x##X, v, p);				\
    }
#include "bitpack16-list.h"
#undef BitPack16_func
