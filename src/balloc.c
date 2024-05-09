/**
 * Includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "balloc.h"



/**
 * Macros and Constants
 */

#define xfree(ptr)  do { free((ptr)); (ptr) = NULL; } while (0)

#if BALLOC_PTR_MD
    #define BALLOC_PTR_OFFSET   (2)
#else // else BALLOC_PTR_MD
    #define BALLOC_PTR_OFFSET   (1)
#endif // BALLOC_PTR_MD


/**
 * Public API
 */

void *balloc_allocate(BuffAlloc *ba, size_t size) {
    size_t i;
    void *ptr = NULL;

    if (!ba->buff) {
        ba->buff = malloc(ba->buff_size);
        if (!ba->buff) {
            BALLOC_LOG_ERR("%s\n", "malloc returned NULL. Not enouph memory on device.\n");
            goto ret;
        }
        ba->end_ptr = ba->buff;
    }

    if ((ba->end_ptr + size) >= (ba->buff + ba->buff_size)) {
        BALLOC_LOG_ERR("Not enouph memory on the buffer for size %zu\n", size);
        goto ret;
    }

#if BALLOC_PTR_MD
    // store new pointer's size before pointer itself
    // with one byte in between.
    *(size_t*)ba->end_ptr = size;
    ba->end_ptr += sizeof(size_t);
    *ba->end_ptr = 0;
    ba->end_ptr += 1;
#endif // BALLOC_PTR_MD

    ptr = (void*)ba->end_ptr;

    ba->end_ptr += size;
    // skip BALLOC_PTR_OFFSET bytes and set them to 0
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
    xfree(ba->buff);
}


void balloc_free_ptr(void *p) {
    ubyte *ptr = (ubyte*)p;
    size_t size = *((size_t*)(ptr-9));
    memset(p-9, 0, size+9);
}


void *balloc_memset(void *p, long c, size_t n) {
#if BALLOC_PTR_MD
    ubyte *ptr = (ubyte*)p;
    size_t size = *((size_t*)(ptr-9));

    if (n > size)
        return NULL;
#else
    BALLOC_LOG_ERR("%s\n", "UNSAFE operation. This can overwrite other data in buffer.");
    BALLOC_LOG_ERR("%s\n", "Define BALLOC_PTR_MD to store pointer size for safe memset operation.");
#endif // BALLOC_PTR_MD

    return memset(p, c, n);
}


void *balloc_memmove(void *p, const void *s, size_t n) {
#if BALLOC_PTR_MD
    ubyte *ptr = (ubyte*)p;
    size_t size = *((size_t*)(ptr-9));

    if (n > size)
        return NULL;
#else
    BALLOC_LOG_ERR("%s\n", "UNSAFE operation. This can overwrite other data in buffer.");
    BALLOC_LOG_ERR("%s\n", "Define BALLOC_PTR_MD to store pointer size for safe memmove operation.");
#endif // BALLOC_PTR_MD

    return memmove(p, s, n);
}
