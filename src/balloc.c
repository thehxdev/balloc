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

/**
 * Includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "balloc.h"



/**
 * Macros and Constants
 */

#define xfree(ptr)  do { free((ptr)); (ptr) = NULL; } while (0)

#if BALLOC_PTR_MD
    #define BALLOC_PTR_OFFSET   (2U)
#else // else BALLOC_PTR_MD
    #define BALLOC_PTR_OFFSET   (1U)
#endif // BALLOC_PTR_MD


/**
 * Public API
 */

void *balloc_allocate(BuffAlloc *ba, size_t size) {
    size_t i;
    void *ptr = NULL;
    ubyte *requested_size = NULL;

    BALLOC_LOG_INF("allocating memory in buffer for size %zu\n", size);
    if (!ba->buff) {
        BALLOC_LOG_INF("%s\n", "buffer is not initialized. initializing buffer");
        ba->buff = malloc(ba->buff_size);
        if (!ba->buff) {
            BALLOC_LOG_ERR("%s\n", "malloc returned NULL. Not enough memory on device\n");
            goto ret;
        }
        BALLOC_LOG_INF("%s\n", "syncing buffer pointer with end pointer");
        ba->end_ptr = ba->buff;
    }

    requested_size = ba->end_ptr + size;

#if BALLOC_PTR_MD
    requested_size += sizeof(size_t) + 1;
#endif // BALLOC_PTR_MD

    if (requested_size >= (ba->buff + ba->buff_size)) {
        BALLOC_LOG_ERR("Not enough memory on the buffer for size %zu\n", size);
        goto ret;
    }

#if BALLOC_PTR_MD
    // store new pointer's size before pointer itself
    // with one byte in between.
    BALLOC_LOG_INF("%s\n", "storing pointer size");
    *(size_t*)ba->end_ptr = size;
    ba->end_ptr += sizeof(size_t);
    *ba->end_ptr = 0;
    ba->end_ptr += 1;
#endif // BALLOC_PTR_MD

    ptr = (void*)ba->end_ptr;

    BALLOC_LOG_INF("adding %zu to end pointer address\n", size);
    ba->end_ptr += size;
    // skip BALLOC_PTR_OFFSET bytes and set them to 0
    BALLOC_LOG_INF("skipping %u bytes after pointer last byte\n", BALLOC_PTR_OFFSET);
    for (i = 0; i < BALLOC_PTR_OFFSET; (i++, ba->end_ptr++))
        *ba->end_ptr = 0;

ret:
    return ptr;
}


void balloc_hexdump(BuffAlloc *ba) {
    ubyte *tmp = ba->buff;
    while (tmp < ba->end_ptr) {
        printf("%x", *tmp);
        tmp += 1;
    }
    putchar('\n');
}


void balloc_free(BuffAlloc *ba) {
    BALLOC_LOG_INF("%s\n", "deinitializing buffer");
    xfree(ba->buff);
    ba->end_ptr = NULL;
}


void balloc_free_ptr(void *p) {
    ubyte *ptr = (ubyte*)p;
    size_t size = *((size_t*)(ptr-9));
    BALLOC_LOG_INF("setting all bytes on address %p with size %zu to zero\n", ptr, size);
    memset(ptr-9, 0, size+9);
}


void *balloc_memset(void *p, long c, size_t n) {
#if BALLOC_PTR_MD
    ubyte *ptr = (ubyte*)p;
    size_t size = *((size_t*)(ptr-9));

    if (n > size) {
        BALLOC_LOG_ERR("Could not copy %zu bytes to a pointer with %zu size\n", n, size);
        return NULL;
    }
#else
    BALLOC_LOG_ERR("%s\n", "UNSAFE operation. This can overwrite other data in buffer");
    BALLOC_LOG_ERR("%s\n", "Set BALLOC_PTR_MD to 1 to store pointer size for safe memset operation");
#endif // BALLOC_PTR_MD

    return memset(p, c, n);
}


void *balloc_memmove(void *p, const void *s, size_t n) {
#if BALLOC_PTR_MD
    ubyte *ptr = (ubyte*)p;
    size_t size = *((size_t*)(ptr-9));

    if (n > size) {
        BALLOC_LOG_ERR("Could not copy %zu bytes to a pointer with %zu size\n", n, size);
        return NULL;
    }
#else
    BALLOC_LOG_ERR("%s\n", "UNSAFE operation. This can overwrite other data in buffer");
    BALLOC_LOG_ERR("%s\n", "Set BALLOC_PTR_MD to 1 to store pointer size for safe memmove operation");
#endif // BALLOC_PTR_MD

    return memmove(p, s, n);
}
