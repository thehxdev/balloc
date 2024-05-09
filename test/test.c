#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/balloc.h"


int main(void) {
    // 0 as buffer size -> use default buffer size (4096 Bytes)
    BuffAlloc alloc = balloc_new(0);

    int *xptr = (int*) balloc_allocate(&alloc, sizeof(int));
    *xptr = 20;

    int *yptr = (int*) balloc_allocate(&alloc, sizeof(int));
    *yptr = 30;

    assert(*xptr == 20);
    assert(*yptr == 30);

    // balloc_hexdump(&alloc);
    balloc_free(&alloc);
    return 0;
}
