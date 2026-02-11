#include "mem-profile.h"
#include <stdio.h>
#include <stdlib.h>

// 初始化内存统计
void mem_profile_init(MemStats *stats) {
    stats->total_alloc = 0;
    stats->total_free = 0;
    stats->current_usage = 0;
    stats->peak_usage = 0;
}

// 记录分配
void mem_profile_alloc(MemStats *stats, size_t size) {
    stats->total_alloc += size;
    stats->current_usage += size;

    if (stats->current_usage > stats->peak_usage) {
        stats->peak_usage = stats->current_usage;
    }
}

// 记录释放
void mem_profile_free(MemStats *stats, size_t size) {
    stats->total_free += size;
    if (stats->current_usage >= size) {
        stats->current_usage -= size;
    }
}

// 获取内存使用率（0-100%）
int mem_profile_usage_percent(MemStats *stats) {
    if (stats->peak_usage == 0) return 0;
    return (int)((stats->current_usage * 100) / stats->peak_usage);
}

// 打印统计
void mem_profile_print(MemStats *stats) {
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║       Memory Profile Statistics                ║\n");
    printf("╠═══════════════════════════════════════════╣\n");
    printf("║ Total Allocated: %10zu KB                ║\n", stats->total_alloc / 1024);
    printf("║ Total Freed:     %10zu KB                 ║\n", stats->total_free / 1024);
    printf("║ Current Usage:  %7zu KB                   ║\n", stats->current_usage / 1024);
    printf("║ Peak Usage:     %7zu KB                    ║\n", stats->peak_usage / 1024);
    printf("║ Usage Rate:     %3d%%                         ║\n", mem_profile_usage_percent(stats));
    printf("╚═══════════════════════════════════════════╝\n");
}

// 占位函数（保留接口）
size_t mem_profile_get_heap_usage(void) {
    return 0;
}

int mem_profile_get_fragmentation(void) {
    return 0;
}

void mem_profile_detailed(void) {
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║         Detailed Memory Statistics         ║\n");
    printf("╠═══════════════════════════════════════════╣\n");
    printf("║ (Detailed stats require valgrind/massif)     ║\n");
    printf("║ Run: valgrind --tool=massif ./q-lite         ║\n");
    printf("╚═══════════════════════════════════════════╝\n\n");
}

void mem_profile_check_warning(MemStats *stats, size_t threshold) {
    if (stats->current_usage > threshold) {
        fprintf(stderr, "[WARNING] Memory usage exceeds threshold: %zu KB > %zu KB\n",
                stats->current_usage / 1024, threshold / 1024);
    }
}
