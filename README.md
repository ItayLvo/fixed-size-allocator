# Fixed-Size Allocator


## Overview
The Fixed-size Allocator is a custom memory allocator written in C, designed to manage fixed-sized memory blocks within a fixed memory pool. This implementation efficiently handles memory allocation, deallocation, and defragmentation, making it a useful component in systems that require fine handling of memory management.
The Allocator is initialized with a size parameter that sets the fixed size of each memory block to be handled by the Allocator.
The data for managing the FSA takes up 8 bytes out of the given memory pool, and doesn't use any additional separate data structures or dynamically allocated memory.




## API

```c
typedef struct fixed_size_allocator fixed_size_allocator_t;
```
Typedef for the allocator.


```c
fixed_size_allocator_t *FSAInitialize(void *memory_pool, size_t block_size, size_t pool_size);
```
Description: Initializes the VSA.
Parameters: Pointer to allocated memory block, the size of each block, and the size of the memory pool.
Returns: Pointer to the allocated block. If failed, returns NULL.


```c
void *FSAAlloc(fixed_size_allocator_t *allocator);
```
Description: Allocates a block of memory.
Parameters: Pointer to allocator.
Returns: Pointer to the allocated memory block. If failed, returns NULL. If no memory blocks are available, returns NULL.



```c
void FSAFree(fixed_size_allocator_t *allocator, void *mem_to_free_ptr)
```
Description: Frees a specific block of memory from the allocator.
Parameters: Pointer to FSA, Pointer to a previously allocated memory block.
Returns: void.



```c
size_t FSACountFree(fixed_size_allocator_t *allocator)
```
Description: Returns the amount of blocks available for allocation.
Parameters: Pointer to an allocator.
Returns: Number of blocks available for allocation. 





## How it works

The fixed-size allocator (FSA) divides a given memory pool into fixed-size blocks and manages them using a **free list**.
The free list is stored within the memory pool itself, without consuming any extra memory.


**Initialization:**

The Allocator is initialized by dividing the memory pool into fixed-size blocks and linking them together to form a free list.

Memory Pool Layout after initialization:
```c
| allocator | block1        | block2        | block3        | block4                 |

|           | (offset to 2) | (offset to 3) | (offset to 4) | 0 ( marks end of list) |
```

**Allocation and Deallocation:**

Allocation:

- First, check if the Free List is empty (by checking if the offset to the next free block is set to 0).
- Retrieve the head of the free list (this block will be allocated to the user).
- Update the new head of the free list to point at the next block after the allocated one.
- After this, the user gets a pointer to the start of the allocated memory block, and the free list offset points to the new head of the free list.



Freeing Memory:

When `FSAfree()` is called, each freed block is inserted back at the head of the free list. This allows it to be quickly re-allocated, keeping the free list management simple and efficient.



Allocation and deallocation efficiency:

Each allocation simply moves the head of the free list to the next available block. This design provides efficient memory allocation, especially when blocks of the same size are frequently requested. **Each allocation and deallocation has O(1) time complexity**.




**Memory Alignment:**

All allocations (and the memory pool itself) are aligned to the word size, ensuring that data within the memory blocks adheres to alignment requirements and optimizing performance.





## Usage example:
```c
#include "fixed_size_allocator.h"

int main() {
    // Define memory pool size and block size
    const size_t pool_size = 1024;
    const size_t block_size = 32;
    const size_t block_count = 10;

    // Allocate a memory pool
    void *memory_pool = malloc(FSASuggestSize(block_size, block_count));
    if (!memory_pool) {
        perror("Failed to allocate memory pool");
        return EXIT_FAILURE;
    }

    // Initialize the allocator with the memory pool
    fixed_size_allocator_t *allocator = FSAInitialize(memory_pool, block_size, pool_size);
    if (!allocator) {
        perror("Failed to initialize allocator");
        free(memory_pool);
        return EXIT_FAILURE;
    }

    // Allocate a block of memory
    void *block1 = FSAAlloc(allocator);
    if (block1) {
        printf("Allocated block at address: %p\n", block1);
    } else {
        printf("Allocation failed\n");
    }

    // Count free blocks available
    size_t free_blocks = FSACountFree(allocator);
    printf("Free blocks available: %zu\n", free_blocks);

    // Free the allocated block
    FSAFree(allocator, block1);
    printf("Freed block at address: %p\n", block1);

    // Clean up
    free(memory_pool);
    return EXIT_SUCCESS;
}
```
