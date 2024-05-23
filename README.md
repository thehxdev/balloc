# Fixed Buffer Memory Allocator

My implementation of Fixed Buffer Memory Allocator in C.


## How it works?

This memory allocator designed to minimize the calls to `malloc` and `free` (in general, system calls) because they
tend to be slow. The allocator allocates a fixed size buffer (4096 Bytes by default) and uses that to store data.

Take a look at `test/test.c` file to see how to use this memory allocator.


## Public API

This section covers balloc's public API

### Constants

There are 3 constants in `src/balloc.h` file that you can modify.

#### `BALLOC_DEBUG`
Set this constant to `1` to enable logging.

#### `BALLOC_PTR_MD`
Set this constant to `1` to store pointers metadata. This will waste a size of `size_t` type for each pointer on the buffer.
But, to do some memory operations safely (since all data lives in a buffer) it's important to know each pointer's size.

#### `DEFAULT_BUFF_SIZE`
Default buffer size. (4096 Bytes)

---

### Functions

#### `balloc_new`
Create and construct a new fixed buffer allocator with type `BuffAlloc`.

#### Other funcitons
Take a look at `src/balloc.h` in `Public API` section for all functions that are available to you.
You can also read `test/test.c` file to see how to use public API and some functions that balloc provides.
