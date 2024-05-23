#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/balloc.h"


int main(void) {
    // 0 as buffer size -> use default buffer size (4096 Bytes)
    BuffAlloc alloc = balloc_new(0);

    long *xptr = (long*) balloc_allocate(&alloc, sizeof(long));
    *xptr = 20;

    long *yptr = (long*) balloc_allocate(&alloc, sizeof(long));
    *yptr = 30;

    assert(*xptr == 20);
    assert(*yptr == 30);

    printf("x: %ld\ny: %ld\n", *xptr, *yptr);
    balloc_free(&alloc);
    return 0;
}
