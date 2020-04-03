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

#define BitPack16_1x16(B, v, p)						\
    do {								\
	B##t x;								\
	B##m m = B##mask(1);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 1));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 2));		\
	x = B##or(x, B##shl(B##and(B##load(v, 3), m), 3));		\
	x = B##or(x, B##shl(B##and(B##load(v, 4), m), 4));		\
	x = B##or(x, B##shl(B##and(B##load(v, 5), m), 5));		\
	x = B##or(x, B##shl(B##and(B##load(v, 6), m), 6));		\
	x = B##or(x, B##shl(B##and(B##load(v, 7), m), 7));		\
	x = B##or(x, B##shl(B##and(B##load(v, 8), m), 8));		\
	x = B##or(x, B##shl(B##and(B##load(v, 9), m), 9));		\
	x = B##or(x, B##shl(B##and(B##load(v, 10), m), 10));		\
	x = B##or(x, B##shl(B##and(B##load(v, 11), m), 11));		\
	x = B##or(x, B##shl(B##and(B##load(v, 12), m), 12));		\
	x = B##or(x, B##shl(B##and(B##load(v, 13), m), 13));		\
	x = B##or(x, B##shl(B##and(B##load(v, 14), m), 14));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 15), m), 15));		\
	B##store(p, 0, x);						\
    } while (0)

#define BitPack16_2x8(B, v, p)						\
    do {								\
	B##t x;								\
	B##m m = B##mask(2);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 2));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 4));		\
	x = B##or(x, B##shl(B##and(B##load(v, 3), m), 6));		\
	x = B##or(x, B##shl(B##and(B##load(v, 4), m), 8));		\
	x = B##or(x, B##shl(B##and(B##load(v, 5), m), 10));		\
	x = B##or(x, B##shl(B##and(B##load(v, 6), m), 12));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 7), m), 14));		\
	B##store(p, 0, x);						\
    } while (0)

#define BitPack16_2x16(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(2);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 2));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 4));		\
	x = B##or(x, B##shl(B##and(B##load(v, 3), m), 6));		\
	x = B##or(x, B##shl(B##and(B##load(v, 4), m), 8));		\
	x = B##or(x, B##shl(B##and(B##load(v, 5), m), 10));		\
	x = B##or(x, B##shl(B##and(B##load(v, 6), m), 12));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 7), m), 14));		\
	B##store(p, 0, x);						\
	y = B##and(B##load(v, 8), m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 9), m), 2));		\
	y = B##or(y, B##shl(B##and(B##load(v, 10), m), 4));		\
	y = B##or(y, B##shl(B##and(B##load(v, 11), m), 6));		\
	y = B##or(y, B##shl(B##and(B##load(v, 12), m), 8));		\
	y = B##or(y, B##shl(B##and(B##load(v, 13), m), 10));		\
	y = B##or(y, B##shl(B##and(B##load(v, 14), m), 12));		\
	y = B##or(y, B##shl(B##clean(B##load(v, 15), m), 14));		\
	B##store(p, 1, y);						\
    } while (0)

#define BitPack16_3x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(3);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 3));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 6));		\
	x = B##or(x, B##shl(B##and(B##load(v, 3), m), 9));		\
	x = B##or(x, B##shl(B##and(B##load(v, 4), m), 12));		\
	y = B##load(v, 5);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 1, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 6), m), 2));		\
	y = B##or(y, B##shl(B##and(B##load(v, 7), m), 5));		\
	B##halfstore(p, 1, y);						\
    } while (0)

#define BitPack16_3x16(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(3);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 3));		\
	x = B##or(x, B##shl(B##and(B##load(v, 2), m), 6));		\
	x = B##or(x, B##shl(B##and(B##load(v, 3), m), 9));		\
	x = B##or(x, B##shl(B##and(B##load(v, 4), m), 12));		\
	y = B##load(v, 5);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 1, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 6), m), 2));		\
	y = B##or(y, B##shl(B##and(B##load(v, 7), m), 5));		\
	y = B##or(y, B##shl(B##and(B##load(v, 8), m), 8));		\
	y = B##or(y, B##shl(B##and(B##load(v, 9), m), 11));		\
	x = B##load(v, 10);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 1, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 11), m), 1));		\
	x = B##or(x, B##shl(B##and(B##load(v, 12), m), 4));		\
	x = B##or(x, B##shl(B##and(B##load(v, 13), m), 7));		\
	x = B##or(x, B##shl(B##and(B##load(v, 14), m), 10));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 15), m), 13));		\
	B##store(p, 2, x);						\
    } while (0)

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

#define BitPack16_6x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(6);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 6));		\
	y = B##load(v, 2);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 4, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 3), m), 2));		\
	B##halfstore(p, 1, y);						\
    } while (0)

#define BitPack16_6x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(6);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 6));		\
	y = B##load(v, 2);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 4, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 3), m), 2));		\
	y = B##or(y, B##shl(B##and(B##load(v, 4), m), 8));		\
	x = B##load(v, 5);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 4, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 6), m), 4));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 7), m), 10));		\
	B##store(p, 2, x);						\
    } while (0)

#define BitPack16_6x16(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(6);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 6));		\
	y = B##load(v, 2);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 4, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 3), m), 2));		\
	y = B##or(y, B##shl(B##and(B##load(v, 4), m), 8));		\
	x = B##load(v, 5);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 4, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 6), m), 4));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 7), m), 10));		\
	B##store(p, 2, x);						\
	y = B##and(B##load(v, 8), m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 9), m), 6));		\
	x = B##load(v, 10);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 4, 2, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 11), m), 2));		\
	x = B##or(x, B##shl(B##and(B##load(v, 12), m), 8));		\
	y = B##load(v, 13);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 2, 4, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 14), m), 4));		\
	y = B##or(y, B##shl(B##clean(B##load(v, 15), m), 10));		\
	B##store(p, 5, y);						\
    } while (0)

#define BitPack16_7x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(7);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 7));		\
	y = B##load(v, 2);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 2, 5, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 3), m), 5));		\
	B##halfstore(p, 1, B##and(y, B##mask(8)));			\
	B##quarterjstore(p, 1, 2, B##clean(B##shr(y, 8), m));		\
    } while (0)

#define BitPack16_7x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(7);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 7));		\
	y = B##load(v, 2);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 2, 5, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 3), m), 5));		\
	x = B##load(v, 4);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 4, 3, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 5), m), 3));		\
	y = B##load(v, 6);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 6, 1, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 7), m), 1));		\
	B##halfstore(p, 3, y);						\
    } while (0)

#define BitPack16_7x16(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(7);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 1), m), 7));		\
	y = B##load(v, 2);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 2, 5, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 3), m), 5));		\
	x = B##load(v, 4);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 4, 3, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 5), m), 3));		\
	y = B##load(v, 6);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 6, 1, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 7), m), 1));		\
	y = B##or(y, B##shl(B##and(B##load(v, 8), m), 8));		\
	x = B##load(v, 9);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(1)), 15));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 1, 6, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 10), m), 6));		\
	y = B##load(v, 11);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 3, 4, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 12), m), 4));		\
	x = B##load(v, 13);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(5)), 11));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 5, 2, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 14), m), 2));		\
	x = B##or(x, B##shl(B##clean(B##load(v, 15), m), 9));		\
	B##store(p, 6, x);						\
    } while (0)

#define BitPack16_8x2(B, v, p)						\
    do {								\
	B##t x;								\
	B##m m = B##mask(8);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 1), m), 8));		\
	B##store(p, 0, x);						\
    } while (0)

#define BitPack16_8x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(8);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 1), m), 8));		\
	B##store(p, 0, x);						\
	y = B##and(B##load(v, 2), m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 3), m), 8));		\
	B##store(p, 1, y);						\
    } while (0)

#define BitPack16_8x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(8);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 1), m), 8));		\
	B##store(p, 0, x);						\
	y = B##and(B##load(v, 2), m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 3), m), 8));		\
	B##store(p, 1, y);						\
	x = B##and(B##load(v, 4), m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 5), m), 8));		\
	B##store(p, 2, x);						\
	y = B##and(B##load(v, 6), m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 7), m), 8));		\
	B##store(p, 3, y);						\
    } while (0)

#define BitPack16_8x16(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(8);						\
	x = B##and(B##load(v, 0), m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 1), m), 8));		\
	B##store(p, 0, x);						\
	y = B##and(B##load(v, 2), m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 3), m), 8));		\
	B##store(p, 1, y);						\
	x = B##and(B##load(v, 4), m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 5), m), 8));		\
	B##store(p, 2, x);						\
	y = B##and(B##load(v, 6), m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 7), m), 8));		\
	B##store(p, 3, y);						\
	x = B##and(B##load(v, 8), m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 9), m), 8));		\
	B##store(p, 4, x);						\
	y = B##and(B##load(v, 10), m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 11), m), 8));		\
	B##store(p, 5, y);						\
	x = B##and(B##load(v, 12), m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 13), m), 8));		\
	B##store(p, 6, x);						\
	y = B##and(B##load(v, 14), m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 15), m), 8));		\
	B##store(p, 7, y);						\
    } while (0)

#define BitPack16_9x2(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(9);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(7)), 9));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 7, 2, m);					\
	B##eighthstore(p, 1, y);					\
    } while (0)

#define BitPack16_9x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(9);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(7)), 9));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 7, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 2), m), 2));		\
	x = B##load(v, 3);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(5)), 11));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 5, 4, m);					\
	B##quarterstore(p, 2, x);					\
    } while (0)

#define BitPack16_9x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(9);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(7)), 9));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 7, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 2), m), 2));		\
	x = B##load(v, 3);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(5)), 11));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 5, 4, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 4), m), 4));		\
	y = B##load(v, 5);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 3, 6, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 6), m), 6));		\
	x = B##load(v, 7);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(1)), 15));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 1, 8, m);					\
	B##halfstore(p, 4, x);						\
    } while (0)

#define BitPack16_9x16(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(9);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(7)), 9));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 7, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 2), m), 2));		\
	x = B##load(v, 3);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(5)), 11));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 5, 4, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 4), m), 4));		\
	y = B##load(v, 5);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 3, 6, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 6), m), 6));		\
	x = B##load(v, 7);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(1)), 15));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 1, 8, m);					\
	y = B##load(v, 8);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(8)), 8));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 8, 1, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 9), m), 1));		\
	x = B##load(v, 10);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 6, 3, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 11), m), 3));		\
	y = B##load(v, 12);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 6, x);						\
	y = B##extract(y, 4, 5, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 13), m), 5));		\
	x = B##load(v, 14);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 7, y);						\
	x = B##extract(x, 2, 7, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 15), m), 7));		\
	B##store(p, 8, x);						\
    } while (0)

#define BitPack16_10x2(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(10);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 6, 4, m);					\
	B##quarterstore(p, 1, y);					\
    } while (0)

#define BitPack16_10x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(10);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 6, 4, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 2), m), 4));		\
	x = B##load(v, 3);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 8, m);					\
	B##halfstore(p, 2, x);						\
    } while (0)

#define BitPack16_10x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(10);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 6, 4, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 2), m), 4));		\
	x = B##load(v, 3);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 8, m);					\
	y = B##load(v, 4);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(8)), 8));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 8, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 5), m), 2));		\
	x = B##load(v, 6);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 4, 6, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 7), m), 6));		\
	B##store(p, 4, x);						\
    } while (0)

#define BitPack16_10x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(10);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 6, 4, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 2), m), 4));		\
	x = B##load(v, 3);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 8, m);					\
	y = B##load(v, 4);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(8)), 8));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 8, 2, m);					\
	y = B##or(y, B##shl(B##and(B##load(v, 5), m), 2));		\
	x = B##load(v, 6);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 4, 6, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 7), m), 6));		\
	B##store(p, 4, x);						\
	y = B##and(B##load(v, 8), m);					\
	x = B##load(v, 9);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 6, 4, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 10), m), 4));		\
	y = B##load(v, 11);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 6, x);						\
	y = B##extract(y, 2, 8, m);					\
	x = B##load(v, 12);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 7, y);						\
	x = B##extract(x, 8, 2, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 13), m), 2));		\
	y = B##load(v, 14);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 8, x);						\
	y = B##extract(y, 4, 6, m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 15), m), 6));		\
	B##store(p, 9, y);						\
    } while (0)

#define BitPack16_11x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(11);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(5)), 11));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 5, 6, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(10)), 6));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 10, 1, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 3), m), 1));		\
	B##halfstore(p, 2, B##and(x, B##mask(8)));			\
	B##quarterjstore(p, 2, 2, B##clean(B##shr(x, 8), B##mask(8)));	\
    } while (0)

#define BitPack16_11x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(11);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(5)), 11));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 5, 6, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(10)), 6));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 10, 1, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 3), m), 1));		\
	y = B##load(v, 4);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 4, 7, m);					\
	x = B##load(v, 5);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(9)), 7));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 9, 2, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 6), m), 2));		\
	y = B##load(v, 7);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 3, 8, m);					\
	B##halfstore(p, 5, y);						\
    } while (0)

#define BitPack16_11x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(11);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(5)), 11));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 5, 6, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(10)), 6));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 10, 1, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 3), m), 1));		\
	y = B##load(v, 4);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 4, 7, m);					\
	x = B##load(v, 5);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(9)), 7));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 9, 2, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 6), m), 2));		\
	y = B##load(v, 7);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 3, 8, m);					\
	x = B##load(v, 8);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 8, 3, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 9), m), 3));		\
	y = B##load(v, 10);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 6, x);						\
	y = B##extract(y, 2, 9, m);					\
	x = B##load(v, 11);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(7)), 9));		\
	B##store(p, 7, y);						\
	x = B##extract(x, 7, 4, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 12), m), 4));		\
	y = B##load(v, 13);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 8, x);						\
	y = B##extract(y, 1, 10, m);					\
	x = B##load(v, 14);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 9, y);						\
	x = B##extract(x, 6, 5, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 15), m), 5));		\
	B##store(p, 10, x);						\
    } while (0)

#define BitPack16_12x2(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(12);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 4, 8, m);					\
	B##halfstore(p, 1, y);						\
    } while (0)

#define BitPack16_12x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(12);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 4, 8, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 8, 4, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 3), m), 4));		\
	B##store(p, 2, x);						\
    } while (0)

#define BitPack16_12x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(12);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 4, 8, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 8, 4, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 3), m), 4));		\
	B##store(p, 2, x);						\
	y = B##and(B##load(v, 4), m);					\
	x = B##load(v, 5);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 4, 8, m);					\
	y = B##load(v, 6);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(8)), 8));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 8, 4, m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 7), m), 4));		\
	B##store(p, 5, y);						\
    } while (0)

#define BitPack16_12x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(12);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 4, 8, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 8, 4, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 3), m), 4));		\
	B##store(p, 2, x);						\
	y = B##and(B##load(v, 4), m);					\
	x = B##load(v, 5);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 4, 8, m);					\
	y = B##load(v, 6);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(8)), 8));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 8, 4, m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 7), m), 4));		\
	B##store(p, 5, y);						\
	x = B##and(B##load(v, 8), m);					\
	y = B##load(v, 9);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 6, x);						\
	y = B##extract(y, 4, 8, m);					\
	x = B##load(v, 10);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 7, y);						\
	x = B##extract(x, 8, 4, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 11), m), 4));		\
	B##store(p, 8, x);						\
	y = B##and(B##load(v, 12), m);					\
	x = B##load(v, 13);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 9, y);						\
	x = B##extract(x, 4, 8, m);					\
	y = B##load(v, 14);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(8)), 8));		\
	B##store(p, 10, x);						\
	y = B##extract(y, 8, 4, m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 15), m), 4));		\
	B##store(p, 11, y);						\
    } while (0)

#define BitPack16_13x2(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(13);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 3, 10, m);					\
	B##halfstore(p, 1, B##and(y, B##mask(8)));			\
	B##eighthjstore(p, 1, 4, B##clean(B##shr(y, 8), B##mask(8)));	\
    } while (0)

#define BitPack16_13x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(13);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 3, 10, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 6, 7, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(9)), 7));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 9, 4, m);					\
	B##quarterstore(p, 3, y);					\
    } while (0)

#define BitPack16_13x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(13);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 3, 10, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 6, 7, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(9)), 7));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 9, 4, m);					\
	x = B##load(v, 4);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(12)), 4));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 12, 1, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 5), m), 1));		\
	y = B##load(v, 6);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 2, 11, m);					\
	x = B##load(v, 7);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(5)), 11));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 5, 8, m);					\
	B##halfstore(p, 6, x);						\
    } while (0)

#define BitPack16_13x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(13);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 3, 10, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 6, 7, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(9)), 7));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 9, 4, m);					\
	x = B##load(v, 4);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(12)), 4));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 12, 1, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 5), m), 1));		\
	y = B##load(v, 6);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 2, 11, m);					\
	x = B##load(v, 7);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(5)), 11));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 5, 8, m);					\
	y = B##load(v, 8);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(8)), 8));		\
	B##store(p, 6, x);						\
	y = B##extract(y, 8, 5, m);					\
	x = B##load(v, 9);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(11)), 5));		\
	B##store(p, 7, y);						\
	x = B##extract(x, 11, 2, m);					\
	x = B##or(x, B##shl(B##and(B##load(v, 10), m), 2));		\
	y = B##load(v, 11);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 8, x);						\
	y = B##extract(y, 1, 12, m);					\
	x = B##load(v, 12);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 9, y);						\
	x = B##extract(x, 4, 9, m);					\
	y = B##load(v, 13);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(7)), 9));		\
	B##store(p, 10, x);						\
	y = B##extract(y, 7, 6, m);					\
	x = B##load(v, 14);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(10)), 6));		\
	B##store(p, 11, y);						\
	x = B##extract(x, 10, 3, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 15), m), 3));		\
	B##store(p, 12, x);						\
    } while (0)

#define BitPack16_14x2(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(14);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 2, 12, m);					\
	B##halfstore(p, 1, B##and(y, B##mask(8)));			\
	B##quarterjstore(p, 1, 2, B##clean(B##shr(y, 8), B##mask(8)));	\
    } while (0)

#define BitPack16_14x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(14);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 2, 12, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 4, 10, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 6, 8, m);					\
	B##halfstore(p, 3, y);						\
    } while (0)

#define BitPack16_14x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(14);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 2, 12, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 4, 10, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 6, 8, m);					\
	x = B##load(v, 4);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 8, 6, m);					\
	y = B##load(v, 5);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(10)), 6));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 10, 4, m);					\
	x = B##load(v, 6);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(12)), 4));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 12, 2, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 7), m), 2));		\
	B##store(p, 6, x);						\
    } while (0)

#define BitPack16_14x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(14);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(2)), 14));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 2, 12, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 4, 10, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(6)), 10));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 6, 8, m);					\
	x = B##load(v, 4);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 8, 6, m);					\
	y = B##load(v, 5);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(10)), 6));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 10, 4, m);					\
	x = B##load(v, 6);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(12)), 4));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 12, 2, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 7), m), 2));		\
	B##store(p, 6, x);						\
	y = B##and(B##load(v, 8), m);					\
	x = B##load(v, 9);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 7, y);						\
	x = B##extract(x, 2, 12, m);					\
	y = B##load(v, 10);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(4)), 12));		\
	B##store(p, 8, x);						\
	y = B##extract(y, 4, 10, m);					\
	x = B##load(v, 11);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 9, y);						\
	x = B##extract(x, 6, 8, m);					\
	y = B##load(v, 12);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(8)), 8));		\
	B##store(p, 10, x);						\
	y = B##extract(y, 8, 6, m);					\
	x = B##load(v, 13);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(10)), 6));		\
	B##store(p, 11, y);						\
	x = B##extract(x, 10, 4, m);					\
	y = B##load(v, 14);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(12)), 4));		\
	B##store(p, 12, x);						\
	y = B##extract(y, 12, 2, m);					\
	y = B##or(y, B##shl(B##clean(B##load(v, 15), m), 2));		\
	B##store(p, 13, y);						\
    } while (0)

#define BitPack16_15x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(15);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 1, 14, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 13, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 3, 12, m);					\
	B##halfstore(p, 3, B##and(y, B##mask(8)));			\
	B##quarterjstore(p, 3, 2, B##clean(B##shr(y, 8), B##mask(8)));	\
    } while (0)

#define BitPack16_15x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(15);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 1, 14, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 13, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 3, 12, m);					\
	x = B##load(v, 4);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 4, 11, m);					\
	y = B##load(v, 5);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(5)), 11));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 5, 10, m);					\
	x = B##load(v, 6);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 6, 9, m);					\
	y = B##load(v, 7);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(7)), 9));		\
	B##store(p, 6, x);						\
	y = B##extract(y, 7, 8, m);					\
	B##halfstore(p, 7, y);						\
    } while (0)

#define BitPack16_15x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	B##m m = B##mask(15);						\
	x = B##and(B##load(v, 0), m);					\
	y = B##load(v, 1);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(1)), 15));		\
	B##store(p, 0, x);						\
	y = B##extract(y, 1, 14, m);					\
	x = B##load(v, 2);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(2)), 14));		\
	B##store(p, 1, y);						\
	x = B##extract(x, 2, 13, m);					\
	y = B##load(v, 3);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(3)), 13));		\
	B##store(p, 2, x);						\
	y = B##extract(y, 3, 12, m);					\
	x = B##load(v, 4);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(4)), 12));		\
	B##store(p, 3, y);						\
	x = B##extract(x, 4, 11, m);					\
	y = B##load(v, 5);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(5)), 11));		\
	B##store(p, 4, x);						\
	y = B##extract(y, 5, 10, m);					\
	x = B##load(v, 6);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(6)), 10));		\
	B##store(p, 5, y);						\
	x = B##extract(x, 6, 9, m);					\
	y = B##load(v, 7);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(7)), 9));		\
	B##store(p, 6, x);						\
	y = B##extract(y, 7, 8, m);					\
	x = B##load(v, 8);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(8)), 8));		\
	B##store(p, 7, y);						\
	x = B##extract(x, 8, 7, m);					\
	y = B##load(v, 9);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(9)), 7));		\
	B##store(p, 8, x);						\
	y = B##extract(y, 9, 6, m);					\
	x = B##load(v, 10);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(10)), 6));		\
	B##store(p, 9, y);						\
	x = B##extract(x, 10, 5, m);					\
	y = B##load(v, 11);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(11)), 5));		\
	B##store(p, 10, x);						\
	y = B##extract(y, 11, 4, m);					\
	x = B##load(v, 12);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(12)), 4));		\
	B##store(p, 11, y);						\
	x = B##extract(x, 12, 3, m);					\
	y = B##load(v, 13);						\
	x = B##or(x, B##shl(B##clean(y, B##mask(13)), 3));		\
	B##store(p, 12, x);						\
	y = B##extract(y, 13, 2, m);					\
	x = B##load(v, 14);						\
	y = B##or(y, B##shl(B##clean(x, B##mask(14)), 2));		\
	B##store(p, 13, y);						\
	x = B##extract(x, 14, 1, m);					\
	x = B##or(x, B##shl(B##clean(B##load(v, 15), m), 1));		\
	B##store(p, 14, x);						\
    } while (0)

#define BitPack16_16x1(B, v, p)						\
    do {								\
	B##t x;								\
	x = B##load(v, 0);						\
	B##store(p, 0, x);						\
    } while (0)

#define BitPack16_16x2(B, v, p)						\
    do {								\
	B##t x, y;							\
	x = B##load(v, 0);						\
	B##store(p, 0, x);						\
	y = B##load(v, 1);						\
	B##store(p, 1, y);						\
    } while (0)

#define BitPack16_16x4(B, v, p)						\
    do {								\
	B##t x, y;							\
	x = B##load(v, 0);						\
	B##store(p, 0, x);						\
	y = B##load(v, 1);						\
	B##store(p, 1, y);						\
	x = B##load(v, 2);						\
	B##store(p, 2, x);						\
	y = B##load(v, 3);						\
	B##store(p, 3, y);						\
    } while (0)

#define BitPack16_16x8(B, v, p)						\
    do {								\
	B##t x, y;							\
	x = B##load(v, 0);						\
	B##store(p, 0, x);						\
	y = B##load(v, 1);						\
	B##store(p, 1, y);						\
	x = B##load(v, 2);						\
	B##store(p, 2, x);						\
	y = B##load(v, 3);						\
	B##store(p, 3, y);						\
	x = B##load(v, 4);						\
	B##store(p, 4, x);						\
	y = B##load(v, 5);						\
	B##store(p, 5, y);						\
	x = B##load(v, 6);						\
	B##store(p, 6, x);						\
	y = B##load(v, 7);						\
	B##store(p, 7, y);						\
    } while (0)

#define BitPack16_16x16(B, v, p)					\
    do {								\
	B##t x, y;							\
	x = B##load(v, 0);						\
	B##store(p, 0, x);						\
	y = B##load(v, 1);						\
	B##store(p, 1, y);						\
	x = B##load(v, 2);						\
	B##store(p, 2, x);						\
	y = B##load(v, 3);						\
	B##store(p, 3, y);						\
	x = B##load(v, 4);						\
	B##store(p, 4, x);						\
	y = B##load(v, 5);						\
	B##store(p, 5, y);						\
	x = B##load(v, 6);						\
	B##store(p, 6, x);						\
	y = B##load(v, 7);						\
	B##store(p, 7, y);						\
	x = B##load(v, 8);						\
	B##store(p, 8, x);						\
	y = B##load(v, 9);						\
	B##store(p, 9, y);						\
	x = B##load(v, 10);						\
	B##store(p, 10, x);						\
	y = B##load(v, 11);						\
	B##store(p, 11, y);						\
	x = B##load(v, 12);						\
	B##store(p, 12, x);						\
	y = B##load(v, 13);						\
	B##store(p, 13, y);						\
	x = B##load(v, 14);						\
	B##store(p, 14, x);						\
	y = B##load(v, 15);						\
	B##store(p, 15, y);						\
    } while (0)

#define BitPack16_func(m, n, X, N)					\
    static inline void bitpack16_##m##x##N(const uint16_t *v, void *p)	\
    {									\
	static_assert(n * X == N, "");					\
	BitPack16_##m##x##n(B16x##X, v, p);				\
    }
#include "bitpack16-list.h"
#undef BitPack16_func
