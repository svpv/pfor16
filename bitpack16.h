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
#include <assert.h>
#include "bitpack16-simd.h"

#define BitPack16_4x4(B, v, p)						\
    do {								\
	B##t x;								\
	B##m m = B##mask(4);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 4));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 8));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 3), m), 12));		\
	B##store(p, 0, x);						\
    } while (0)

#define BitPack16_4x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(4);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 4));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 8));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 3), m), 12));		\
	B##store(p, 0, x);						\
	y = B##and(B##load(v, 4), m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 5), m), 4));		\
	y = B##or(y, B##shl(B##and(B##load(v, 6), m), 8));		\
	y = B##or(y, B##shl(B##clean(B##load(v, 7), m), 12));		\
	B##store(p, 1, y);						\
    } while (0)

#define BitPack16_4x16(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(4);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 4));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 8));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 3), m), 12));		\
	B##store(p, 0, x);						\
	y = B##and(B##load(v, 4), m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 5), m), 4));		\
	y = B##or(y, B##shl(B##and(B##load(v, 6), m), 8));		\
	y = B##or(y, B##shl(B##clean(B##load(v, 7), m), 12));		\
	B##store(p, 1, y);						\
	x = B##and(B##load(v, 8), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 9), m), 4));		\
	x = B##or(x, B##shl(B##and(B##load(v, 10), m), 8));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 11), m), 12));		\
	B##store(p, 2, x);						\
	y = B##and(B##load(v, 12), m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 13), m), 4));		\
	y = B##or(y, B##shl(B##and(B##load(v, 14), m), 8));		\
	y = B##or(y, B##shl(B##clean(B##load(v, 15), m), 12));		\
	B##store(p, 3, y);						\
    } while (0)

#define BitPack16_5x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(5);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 5));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 10));		\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 1, 4, m);					\
	B##quarterstore(p, 1, y);					\
    } while (0)

#define BitPack16_5x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(5);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 5));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 10));		\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 1, 4, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 4), m), 4));		\
	y = B##or(y, B##shl(B##and(B##load(v, 5), m), 9));		\
	x = B##load(v, 6);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 3, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 7), m), 3));		\
	B##halfstore(p, 2, x);						\
    } while (0)

#define BitPack16_5x16(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(5);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 5));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 10));		\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 1, 4, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 4), m), 4));		\
	y = B##or(y, B##shl(B##and(B##load(v, 5), m), 9));		\
	x = B##load(v, 6);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 3, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 7), m), 3));		\
	x = B##or(x, B##shl(B##and(B##load(v, 8), m), 8));		\
	y = B##load(v, 9);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 3, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 10), m), 2));		\
	y = B##or(y, B##shl(B##and(B##load(v, 11), m), 7));		\
	x = B##load(v, 12);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 4, 1, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 13), m), 1));		\
	x = B##or(x, B##shl(B##and(B##load(v, 14), m), 6));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 15), m), 11));		\
	B##store(p, 4, x);						\
    } while (0)

#define BitPack16_func(m, n, X, N)					\
    static inline void bitpack16_##m##x##N(const uint16_t *v, void *p)	\
    {									\
	static_assert(n * X == N, "");					\
	BitPack16_##m##x##n(B16x##X, v, p);				\
    }
#include "bitpack16-list.h"
#undef BitPack16_func
