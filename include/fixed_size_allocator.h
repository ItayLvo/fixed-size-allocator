#ifndef __FIXED_SIZE_ALLOCATOR_H__
#define __FIXED_SIZE_ALLOCATOR_H__

#include <stddef.h> /* size_t */

typedef struct fixed_size_allocator fixed_size_allocator_t;

/*
description: Initializes new fixed size allocator. Allocates memory dynamically.
input: size of each block and number of blocks that the allocator will handle.
return: a pointer to a allocator. if failed, returns NULL.
time complexity: O(1)
space complexity: O(n)
*/
fixed_size_allocator_t *FSAInitialize(void *memory_pool, size_t block_size, size_t pool_size);

/*
description: allocates block_count number of fixed size blocks, by request from the user. Doesn't allocate any additional memory from the Heap.
input: Pointer to allocator, size_t block_count for number of memory blocks to be allocated.
return: pointer to memory allocated, aligned
time complexity: O(1)
space complexity: O(1)
*/
void *FSAAlloc(fixed_size_allocator_t *allocator);

/*
description: Frees a specific block of memory from the allocator.
input: Pointer to allocator, pointer to memory block.
return: void.
time complexity: O(1)
space complexity: O(1)
*/
void FSAFree(fixed_size_allocator_t *allocator, void *mem_to_free_ptr);

/*
description: Returns number of free blocks
input: Pointer to allocator
return: number of free blocks.
time complexity: O(n)
space complexity: O(1)
*/
size_t FSACountFree(fixed_size_allocator_t *allocator);

/*
description: Calculates the suggested size to pass the the Initialize() function, to consider system requirements and overhead by the API functions
input: block size and block count
return: (size_t) number of bytes suggested to be allocated by the user
time complexity: O(1)
space complexity: O(1)
*/
size_t FSASuggestSize(size_t block_size, size_t block_count);

#endif /* __FIXED_SIZE_ALLOCATOR_H__ */
