#include "allocation/stalloc.h"

void st_alloc_init(STAllocator *allocator, void *start) {
    allocator->start = start;
    allocator->offset = 0;
}

void *st_alloc(STAllocator *allocator, size_t size, size_t align) {
    size_t mask = align - 1;
    size_t aligned_offset = (allocator->offset + mask) & ~mask;

    if (!allocator->start) return NULL;

    void *alloc_ptr = (char *)allocator->start + aligned_offset;
    allocator->offset = aligned_offset + size;
    return alloc_ptr;
}