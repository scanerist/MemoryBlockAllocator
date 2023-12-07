# MemoryPoolsAllocator

## Project Description

MemoryPoolsAllocator is a memory allocator that works based on pre-allocated blocks of memory and meets the requirements for allocators.
The project aims to implement an allocator and compare its performance with the standard allocator when interacting with containers.
The obtained results will be presented on a graph and are part of the task that affects the final grade.

## Working Principle

The allocator uses pre-allocated blocks of memory that are divided into pools and pieces of the same size.
Pools can only allocate and deallocate memory in pieces and have information about free and occupied pieces.
When allocating N-byte memory, the allocator searches for the most suitable pool to optimally allocate memory.
If this is not possible, the `std::bad_alloc` exception is thrown.
When freed through the allocator, memory pieces become available for use again.

## Implementation

The sizes of pools, their number, and the sizes of pieces can be parameterized (at runtime).
The allocator does not necessarily have to own pools. The design is up to the developer's discretion.
