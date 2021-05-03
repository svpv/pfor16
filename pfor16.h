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

#pragma once
#include <stddef.h>
#include <stdint.h>

// The maximum compressed size.
#define PFOR16_ENCBOUND(n) (2UL*(n) + ((n)/256UL) + 1)

// Compress v[n] into out.  Returns the compressed size.
size_t pfor16enc(uint16_t *v, size_t n, void *out);

// Uncompress src[len] into v[n].  The exact value of n must be known
// in advance.  Returns the the number of bytes consumed from src (which
// can be fewer than len), or 0 on error.
size_t pfor16dec(const void *src, size_t len, uint16_t *v, size_t n);
