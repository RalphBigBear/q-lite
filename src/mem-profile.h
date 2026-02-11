#ifndef MEM_PROFILE_H
#define MEM_PROFILE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// 内存统计
typedef struct {
    size_t total_alloc;
    size_t total_free;
    size_t current_usage;
    size_t peak_usage;
} MemStats;

// 函数声明
void mem_profile_init(MemStats *stats);
void mem_profile_alloc(MemStats *stats, size_t size);
void mem_profile_free(MemStats *stats, size_t size);
int mem_profile_usage_percent(MemStats *stats);
void mem_profile_print(MemStats *stats);
size_t mem_profile_get_heap_usage(void);
int mem_profile_get_fragmentation(void);
void mem_profile_detailed(void);
void mem_profile_check_warning(MemStats *stats, size_t threshold);

#endif
