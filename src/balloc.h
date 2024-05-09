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

#define DEFAULT_BUFF_SIZE (1<<12)


#define balloc_new(buff_size)   \
    (BuffAlloc) { NULL, NULL, (buff_size) ? buff_size : DEFAULT_BUFF_SIZE }

#if BALLOC_DEBUG
    #define BALLOC_LOG_ERR(format, ...) \
        (void)fprintf(stderr, "[ERROR] %s: " format, __FUNCTION__, __VA_ARGS__)
#else
    #define BALLOC_LOG_ERR(format, ...)
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

void *balloc_allocate(BuffAlloc *ba, size_t size);

void balloc_hexdump(BuffAlloc *ba);

void balloc_free(BuffAlloc *ba);


#endif // BALLOC_H
