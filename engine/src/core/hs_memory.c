#include "hs_memory.h"

#include "logger.h"
#include "platform/platform.h"

// TODO: custom string lib
#include <string.h>
#include <stdio.h>

struct memory_stats {
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static const char* memory_tag_names[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN:    ",
    "ARRAY:      ",
    "DARRAY:     ",
    "DICT:       ",
    "RING_QUEUE: ",
    "BST:        ",
    "STRING:     ",
    "APPLICATION:",
    "JOB:        ",
    "TEXTURE:    ",
    "MAT_INST:   ",
    "RENDERER:   ",
    "GAME:       ",
    "TRANSFORM:  ",
    "ENTITY:     ",
    "ENTITY_NODE:",
    "SCENE:      "
};

static struct memory_stats stats;

void init_memory() {
    platform_zero_memory(&stats, sizeof(stats));
}

void shutdown_memory() {
    // TODO: memory cleanup when needed
}

void* hs_alloc(u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        HS_WARN("hs_alloc called with MEMORY_TAG_UNKNOWN, reassign tag.");
    }

    // Update stats
    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    // TODO: memory alignment
    void* block = platform_alloc(size, FALSE);
    platform_zero_memory(block, size);

    return block;
}

void hs_free(void* block, u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        HS_WARN("hs_free called with MEMORY_TAG_UNKNOWN, reassign tag.");
    }

    // Update stats
    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    // TODO: memory alignment
    platform_free(block, FALSE);
}

void* hs_zero_memory(void* block, u64 size) {
    return platform_zero_memory(block, size);
}

void* hs_copy_memory(void* dest, const void* source, u64 size) {
    return platform_copy_memory(dest, source, size);
}

void* hs_set_memory(void* dest, i32 value, u64 size) {
    return platform_set_memory(dest, value, size);
}

char* get_memory_usage() {
    const u64 kib = 1024;
    const u64 mib = 1024 * 1024;
    const u64 gib = 1024 * 1024 * 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = strlen(buffer);
    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; i++) {
        char unit[4] = "XiB";
        float amount = 0.0f;
        if (stats.tagged_allocations[i] >= gib) {
            amount = (float)stats.tagged_allocations[i] / (float)gib;
            unit[0] = 'G';
        } else if (stats.tagged_allocations[i] >= mib) {
            amount = (float)stats.tagged_allocations[i] / (float)mib;
            unit[0] = 'M';
        } else if (stats.tagged_allocations[i] >= kib) {
            amount = (float)stats.tagged_allocations[i] / (float)kib;
            unit[0] = 'K';
        } else {
            amount = (float)stats.tagged_allocations[i];
            unit[0] = 'B';
            unit[1] = 0; // null terminator
        }
        
        i32 length = snprintf(buffer + offset, 8000 - offset, "  %s %.2f %s\n", memory_tag_names[i], amount, unit);
        offset += length;
    }
    char* result = _strdup(buffer);
    return result;
}