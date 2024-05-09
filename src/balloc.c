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

#define BALLOC_PTR_OFFSET   (2)


/**
 * Public API
 */

void *balloc_allocate(BuffAlloc *ba, size_t size) {
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

    ptr = (void*)ba->end_ptr;

    ba->end_ptr += size;
    // skip 2 bytes and set them to 0
    memset(ba->end_ptr, 0, BALLOC_PTR_OFFSET);
    ba->end_ptr += BALLOC_PTR_OFFSET;

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
