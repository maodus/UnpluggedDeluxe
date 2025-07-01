#ifndef ST_ALLOCATOR_H
#define ST_ALLOCATOR_H

#include <stddef.h> // for size_t

typedef struct {
    void *start;
    size_t offset;
    size_t size;
} STAllocator;

void st_alloc_init(STAllocator *allocator, void *start, size_t size);
void *st_alloc(STAllocator *allocator, size_t size, size_t align);

#endif // ST_ALLOCATOR_H