#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */

#include "fixed_size_allocator.h"		/* FSA functions, typedefs */

/* macro for word size, which is 8 bytes on my system */
#define WORD_SIZE sizeof(void *)

/* declaring the managerial struct */
struct fixed_size_allocator
{
	/* holds the offset from the start of the memory pool to the next available block. */
	/* this offset acts as a pointer to the head of the free list. */
	size_t free_list_offset;
};

/* forward decleration of static functions */
static size_t AlignBlockSize(size_t block_size);


fixed_size_allocator_t *FSAInitialize(void *memory_pool, size_t block_size, size_t pool_size)
{
	fixed_size_allocator_t *allocator = (fixed_size_allocator_t *)memory_pool;
	size_t index = 0;
	size_t total_blocks = 0;
	size_t *current_block = NULL;
	size_t offset = 0;
	
	assert(memory_pool);
	
	block_size = AlignBlockSize(block_size);
	
	/* calculate the number of memory blocks by dividing the usable part of the pool by the aligned block size */
	total_blocks = (pool_size - sizeof(fixed_size_allocator_t)) / block_size;
	
	/* setting the head of the free list to start after the managerial struct */
	allocator->free_list_offset = sizeof(fixed_size_allocator_t);
	
	/* initialize the free list - (*current_block) holds the offset to the next block */
	offset = allocator->free_list_offset;
	current_block = (size_t *)((char *)allocator + offset);
	for (index = 0; index < total_blocks - 1; ++index)
	{
		offset += block_size;
		*current_block = offset;
		current_block = (size_t *)((char *)allocator + offset);
	}
	*current_block = 0;
	
	return allocator;
}


void *FSAAlloc(fixed_size_allocator_t *allocator)
{
	size_t *allocated_block = NULL;
	
	assert(allocator);
	
	/* if the offset is 0, that means there are no blocks available */
	if (0 == allocator->free_list_offset)
	{
		return NULL;
	}
	
	/* set the allocated block to the current head of the free list */
	allocated_block = (void *)((char *)allocator + allocator->free_ist_offset);
	
	/* update the free lisr offset to point at the next free block */
	allocator->free_list_offset = *allocated_block;

	return allocated_block;
}

void FSAFree(fixed_size_allocator_t *allocator, void *mem_to_free_ptr)
{
	size_t *ptr_to_free_addrs = NULL;
	
	assert(allocator);
	assert(mem_to_free_ptr);
	
	
	ptr_to_free_addrs = (size_t *)mem_to_free_ptr;
	
	/* point the newly freed block to the head of the free list */
	*ptr_to_free_addrs = allocator->free_list_offset;
	
	/* set the head of the free list to be the newly freed block */
	allocator->free_list_offset = (size_t)ptr_to_free_addrs - (size_t)allocator;
}


size_t FSACountFree(fixed_size_allocator_t *allocator)
{
	size_t current_offset = 0;
	size_t count = 0;
	
	assert(allocator);
	
	/* start at the head of the free list and traverse the nodes until end of list */
	current_offset = allocator->free_list_offset;
	while (0 != current_offset)
	{
		current_offset = *(size_t *)((char *)allocator + current_offset);
		++count;
	}
	
	return count;
}


size_t FSASuggestSize(size_t block_size, size_t block_count)
{
	block_size = AlignBlockSize(block_size);
	
	/* managment struct size + number of blocks * block size */
	return (sizeof(fixed_size_allocator_t) + (block_count * block_size));
}




static size_t AlignBlockSize(size_t block_size)
{
	/* word size is the smallest possible block size */
	/* the reason is both alignment, and to be able to hold the offset of the next block */
	if (block_size < WORD_SIZE)
	{
			block_size = WORD_SIZE;
			return block_size;
	}
	
	/* if the block size is aligned (is a multiple of word size) */
	if (block_size % WORD_SIZE == 0)
	{
		return block_size;
	}
	
	/* if the block size is not a multiple of word size - round it the the next nearest multiple of word size */
	block_size = (((block_size / WORD_SIZE) + 1) * WORD_SIZE);
	
	return block_size;
}
