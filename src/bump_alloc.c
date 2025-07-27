#include "allocation/bump_alloc.h"

void bump_alloc_init(BumpAllocator *allocator, void *start, size_t size) {
    allocator->start = start;
    allocator->offset = 0;
    allocator->size = size;
}

void *bump_alloc(BumpAllocator *allocator, size_t size, size_t align) {
	const size_t mask = align - 1;
	const size_t aligned_offset = (allocator->offset + mask) & ~mask;
	const size_t new_offset = aligned_offset + size;

	if (!allocator->start || new_offset > allocator->size){
		return NULL;
	}

	void *alloc_ptr = (char *)allocator->start + aligned_offset;
	allocator->offset = aligned_offset + size;
	return alloc_ptr;
}