#include "mem.h"

typedef struct FreeBlock {
    struct FreeBlock* next; // Pointer to the next free block
    size_t size;            // Size of the free block
} FreeBlock;

FreeBlock* free_list = NULL;


void memory_copy(uint8_t *source, uint8_t *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void initialize_memory_pool(uint8_t* pool_start, size_t pool_size) {
    free_list = (FreeBlock*)pool_start;
    free_list->next = NULL;
    free_list->size = pool_size - sizeof(FreeBlock);
}


/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined on the Makefile */
uint32_t free_mem_addr = 0x10000;
/* Implementation is just a pointer to some free memory which
 * keeps growing */
uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}

void kfree(void* ptr) {
    if (ptr == NULL) {
        // Don't do anything if a null pointer is passed
        return;
    }

    // Cast the pointer to FreeBlock to get the size of the block
    FreeBlock* header = (FreeBlock*)((uint8_t*)ptr - sizeof(FreeBlock));
    header->next = free_list;
    free_list = header;

    // Merge contiguous free blocks
    FreeBlock* current = free_list;
    while (current != NULL && current->next != NULL) {
        if ((uint8_t*)current + sizeof(FreeBlock) + current->size == (uint8_t*)current->next) {
            current->size += sizeof(FreeBlock) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}
