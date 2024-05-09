# Buffer Allocator

My (First Try) impelementation of Fixed Buffer Memory Allocator in C.


## How it works?

This memory allocator designed to minimize the calls to `malloc` and `free` (in general, system calls) because they
tend to be slow. The allocator allocates a fixed size buffer (4096 Bytes by default) and uses that to store data.

Take a look at `test/test.c` file to see how to use this memory allocator.
