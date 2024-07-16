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
#include <sys/mman.h>
#include "balloc.h"



/**
 * Macros and Constants
 */

#define xfree(ptr)  do { free((ptr)); (ptr) = NULL; } while (0)



/**
 * Private API
 */

static Buff *balloc_create_buffer(size_t size) {
    BALLOC_LOG_INF("creating a new buffer with size %zu\n", size);
    Buff *b = malloc(sizeof(*b));
    if (!b)
        goto ret;

    // if buffer size is bigger than page size,
    // use mmap for allocating memory.
    b->ptr = mmap(NULL, size,
                  PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS,
                  -1, 0);

    if (b->ptr == MAP_FAILED) {
        BALLOC_LOG_ERR("%s\n", "mmap failed");
        xfree(b);
        goto ret;
    }

    BALLOC_LOG_INF("new buffer address: %p\n", b->ptr);

ret:
    return b;
}


/**
 * Public API
 */

BuffAlloc balloc_new() {
    BALLOC_LOG_INF("%s\n", "creating new buffer allocator");
    BuffAlloc alloc = (BuffAlloc) {
        .buffers = { NULL, NULL },
        .end_ptr = NULL,
        .buff_size = DEFAULT_BUFF_SIZE,
    };

    Buff *b = balloc_create_buffer(alloc.buff_size);
    alloc.buffers.head = b;
    alloc.buffers.tail = b;
    if (b)
        alloc.end_ptr = alloc.buffers.tail->ptr;

    return alloc;
}


void *balloc_allocate(BuffAlloc *ba, size_t size) {
    void *ptr = NULL;
    Buff *buff = ba->buffers.tail;
    ubyte *required_size = NULL;

    if (!buff)
        return NULL;

    BALLOC_LOG_INF("allocating memory in buffer for size %zu\n", size);

    required_size = ba->end_ptr + size;
#if BALLOC_PTR_MD
    required_size += sizeof(size_t);
#endif // BALLOC_PTR_MD

    if (required_size > (buff->ptr + ba->buff_size)) {
        buff = balloc_create_buffer(ba->buff_size);
        buff->prev = ba->buffers.tail;
        ba->buffers.tail->next = buff;
        ba->buffers.tail = buff;
        ba->end_ptr = buff->ptr;
    }

#if BALLOC_PTR_MD
    // store new pointer's size before pointer itself
    // with one byte in between.
    BALLOC_LOG_INF("%s\n", "storing pointer size");
    *(size_t*)ba->end_ptr = size;
    ba->end_ptr += sizeof(size_t);
#endif // BALLOC_PTR_MD

    ptr = (void*)ba->end_ptr;
    BALLOC_LOG_INF("adding %zu to end pointer address\n", size);
    ba->end_ptr += size;

    return ptr;
}


void balloc_free(BuffAlloc *ba) {
    BALLOC_LOG_INF("%s\n", "deinitializing buffer");
    Buff *tmp = ba->buffers.head, *next;
    while (tmp) {
        munmap(tmp->ptr, ba->buff_size);
        next = tmp->next;
        xfree(tmp);
        tmp = next;
    }
}


void balloc_free_ptr(void *p) {
    ubyte *ptr = (ubyte*)p;
    size_t offset = sizeof(size_t);
    size_t size = *((size_t*)(ptr - offset));

    BALLOC_LOG_INF("setting all bytes on address %p with size %zu to zero\n", ptr, size);
    memset(ptr - offset, 0, size + offset);
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
