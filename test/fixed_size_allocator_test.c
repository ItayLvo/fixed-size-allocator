#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "fixed_size_allocator.h"	/* fixed_size_allocator_t and FSA functions */

int FSATestHeap();
int FSATestStack();


int main()
{
	FSATestHeap();
	FSATestStack();
	
	return 0;
}

int FSATestHeap()
{
	size_t block_size = 8, block_count = 10;
	size_t suggested_size = FSASuggestSize(block_size, block_count);
	int *pool = malloc(suggested_size);
	int *test_ptr_1 = NULL;
	
	fixed_size_allocator_t * allocator = FSAInitialize(pool, block_size, suggested_size);
	
	printf("suggested (expected result is 88) = %ld\n", suggested_size);
	printf("countfree (expected result is 10) = %ld\n", FSACountFree(allocator));
	
	test_ptr_1 = FSAAlloc(allocator);
	*test_ptr_1 = 5;
	printf("printing the data after alloc, exptected result = 5:\t%d\n", *test_ptr_1);
	printf("countfree (expected result is 9) = %ld\n", FSACountFree(allocator));
	
	FSAFree(allocator, test_ptr_1);
	printf("countfree (expected result is 10) = %ld\n", FSACountFree(allocator));
	
	free(pool);
	
	return 0;
}


int FSATestStack()
{
	size_t block_size = 16, block_count = 5;
	size_t suggested_size = FSASuggestSize(block_size, block_count);
	int pool[88];
	int *test_ptr_1 = NULL;
	
	fixed_size_allocator_t * allocator = FSAInitialize(pool, block_size, suggested_size);
	
	printf("\n\nsuggested (expected result is 88) = %ld\n", suggested_size);
	printf("countfree (expected result is 5) = %ld\n", FSACountFree(allocator));
	
	test_ptr_1 = FSAAlloc(allocator);
	*test_ptr_1 = 999;
	printf("printing the data after alloc, exptected result = 999:\t%d\n", *test_ptr_1);
	printf("countfree (expected result is 4) = %ld\n", FSACountFree(allocator));
	
	FSAFree(allocator, test_ptr_1);
	printf("countfree (expected result is 5) = %ld\n", FSACountFree(allocator));
	
	return 0;
}
