// SPDX-License-Identifier: BSD-3-Clause

#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <internal/essentials.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <internal/syscall.h>
extern struct mem_list mem_list_head;
static int is_init = 0;
#define ALIGNMENT 16

#ifndef MMAP_THRESHOLD
#define MMAP_THRESHOLD (128 * 1024)
#endif

void ensure_list_init(){
	if(!is_init){
		mem_list_init();
		is_init = 1;
	}
}

void *malloc(size_t size)
{
	/* TODO: Implement malloc(). */
	ensure_list_init();
	size = (size + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
	if (size >= MMAP_THRESHOLD) {
		void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
		                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (ptr == MAP_FAILED) {
			errno = ENOMEM;
			return NULL;
		}
		if (mem_list_add(ptr, size) == -1) {
			munmap(ptr, size);
			errno = ENOMEM;
			return NULL;
		}
		return ptr;
	}
	struct mem_list *free_blk = mem_list_find_free(size);
    if (free_blk != NULL) {
        free_blk->free = 0;
        return free_blk->start;
    }
	void *current_brk = (void *)syscall(__NR_brk, 0);
    if (current_brk == (void *)-1) {
        errno = ENOMEM;
        return NULL;
    }
	void *new_brk = (void *)syscall(__NR_brk, (unsigned char *)current_brk + size);
    if (new_brk == (void *)-1 || ((unsigned char*)new_brk < (unsigned char *)(current_brk + size))) {
        errno = ENOMEM;
        return NULL;
    }
	if (mem_list_add(current_brk, size) == -1) {
        syscall(__NR_brk, current_brk);
        errno = ENOMEM;
        return NULL;
    }
    return current_brk;
}

void *calloc(size_t nmemb, size_t size)
{
	/* TODO: Implement calloc(). */
	ensure_list_init();
	size_t total_size = (nmemb*size + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
	struct mem_list *free_blk = mem_list_find_free(total_size);
    if (free_blk != NULL) {
        free_blk->free = 0;
        return free_blk->start;
    }
	void *current_brk = (void *)syscall(__NR_brk, 0);
    if (current_brk == (void *)-1) {
        errno = ENOMEM;
        return NULL;
    }
	void *new_brk = (void *)syscall(__NR_brk, (unsigned char *)current_brk + total_size);
    if (new_brk == (void *)-1 || ((unsigned char*)new_brk < (unsigned char *)(current_brk + total_size))) {
        errno = ENOMEM;
        return NULL;
    }
	if (mem_list_add(current_brk, total_size) == -1) {
        syscall(__NR_brk, current_brk);
        errno = ENOMEM;
        return NULL;
    }
	memset(current_brk, 0, total_size);
    return current_brk;
}

void combine_blocks(void* ptr){
    struct mem_list* curr = mem_list_find(ptr);
    if (curr == NULL || !curr->free) return;
    struct mem_list* next = curr->next;
    while (next != &mem_list_head && next->free) {
        void *curr_end = (char *)curr->start + curr->len;
        if (curr_end == next->start) {
            curr->len += next->len;
            struct mem_list* to_delete = next;
            next = next->next;
            mem_list_del(to_delete->start);
        } else {
            break;
        }
    }
}

void check_last_block(void* ptr){
	struct mem_list *freed_block = mem_list_find(ptr);
    if (freed_block == NULL || !freed_block->free) return;
	void *block_end = (unsigned char *)freed_block->start + freed_block->len;
    void *current_brk = (void *)syscall(__NR_brk, 0);
    if (block_end == current_brk) {
        syscall(__NR_brk, freed_block->start);
        mem_list_del(freed_block->start);
    }
}

void free(void *ptr)
{
	if (!ptr) return;
	if (mem_list_mark_free(ptr) == -1){
		errno = EINVAL;
	}
	struct mem_list *block = mem_list_find(ptr);
	if (block->len >= MMAP_THRESHOLD) {
		munmap(block->start, block->len);
		mem_list_del(block->start);
		return;
	}
	combine_blocks(ptr);
	check_last_block(ptr);
}

void *realloc(void *ptr, size_t size)
{
	/* TODO: Implement realloc(). */
    void* addr = malloc(size);
    struct mem_list *block = mem_list_find(ptr);
    memcpy(addr, ptr, block->len);
    return addr;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	/* TODO: Implement reallocarray(). */
    int overfl = 0;
    size_t prod = nmemb * size;
    if ((long)prod < 0)
        overfl = 1;
    // if (size != 0 && nmemb > SIZE_MAX / size)
    //     return NULL;
    if (overfl) return NULL;
    return realloc(ptr, prod);
}
