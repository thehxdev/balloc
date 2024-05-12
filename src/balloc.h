/**
 * balloc - Fixed Buffer Memory Allocator
 * Author: Hossein Khosravi (@thehxdev)
 *
 *
 * MIT License
 *
 * Copyright (c) 2024 Hossein Khosravi (@thehxdev)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef BALLOC_H
#define BALLOC_H


/**
 * Includes
 */

#include <stddef.h>



/**
 * Macros and Constants
 */

// Set this constant to 1 to enable logging
#define BALLOC_DEBUG    0

// Store pointer metadata (it's size).
// This will take a size of `size_t` for each pointer.
// Pointer's size is required for some functionalities.
#define BALLOC_PTR_MD   0

#define DEFAULT_BUFF_SIZE (1<<12)


#define balloc_new(buff_size)   \
    ((BuffAlloc) { NULL, NULL, (buff_size) ? buff_size : DEFAULT_BUFF_SIZE })

#if BALLOC_DEBUG
    #define BALLOC_LOG_ERR(format, ...) \
        (void)fprintf(stderr, "[ERROR] %s: " format, __FUNCTION__, __VA_ARGS__)
    #define BALLOC_LOG_INF(format, ...) \
        (void)fprintf(stderr, "[INFO] %s: " format, __FUNCTION__, __VA_ARGS__)
#else
    #define BALLOC_LOG_ERR(format, ...)
    #define BALLOC_LOG_INF(format, ...)
#endif //BALLOC_DEBUG


/**
 * Types
 */

typedef char byte;
typedef unsigned char ubyte;

typedef struct __buff_alloc {
    ubyte *buff;

    ubyte *end_ptr;

    size_t buff_size;
} BuffAlloc;



/**
 * Public API
 */

// allocate memory on the buffer
void *balloc_allocate(BuffAlloc *ba, size_t size);

void balloc_hexdump(BuffAlloc *ba);

// free the buffer
void balloc_free(BuffAlloc *ba);

#if BALLOC_PTR_MD
// free one pointer that allocated on the buffer
void balloc_free_ptr(void *p);
#endif // BALLOC_PTR_MD

// memset operation on the pointer that allocated on the buffer
void *balloc_memset(void *p, long c, size_t n);

// memmove operation on the pointer that allocated on the buffer
void *balloc_memmove(void *p, const void *s, size_t n);


#endif // BALLOC_H
