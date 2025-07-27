#ifndef BUMP_ALLOCATOR_H
#define BUMP_ALLOCATOR_H

#include <stddef.h> // for size_t

typedef struct {
    void *start;
    size_t offset;
    size_t size;
} BumpAllocator;

void bump_alloc_init(BumpAllocator *allocator, void *start, size_t size);
void *bump_alloc(BumpAllocator *allocator, size_t size, size_t align);

#endif // BUMP_ALLOCATOR_H