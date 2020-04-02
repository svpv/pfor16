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

#define BitPack16_func(m, n, X, N)					\
    static inline void bitpack16_##m##x##N(const uint16_t *v, void *p)	\
    {									\
	static_assert(n * X == N, "");					\
	BitPack16_##m##x##n(B16x##X, v, p);				\
    }
#include "bitpack16-list.h"
#undef BitPack16_func
