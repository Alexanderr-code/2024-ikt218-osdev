#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <libc/stdint.h>
#include <libc/stddef.h>

typedef struct {
	uint8_t status;
	uint32_t size;
} alloc_t;

extern void init_kernel_memory();
extern void print_memory_layout();

extern void paging_init();
extern void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);

extern char* pmalloc(size_t size); /* page aligned alloc */
extern char* malloc(size_t size);
extern void free(void *mem);

extern void* memcpy(const void* dest, const void* src, size_t  );
extern void* memset (void * ptr, int value, size_t num );
extern void* memset16 (void *ptr, uint16_t value, size_t num);

#endif