#pragma once

#include "defines.h"

// Enum for memory tags
typedef enum memory_tag {
    // Temporary, should be assigned one of these tags or create a new one
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,

    // Add new tags above this line
    MEMORY_TAG_MAX_TAGS
} memory_tag;

// ---------- Methods ----------
HS_API void init_memory();
HS_API void shutdown_memory();

HS_API void* hs_alloc(u64 size, memory_tag tag);

HS_API void hs_free(void* block, u64 size, memory_tag tag);

HS_API void* hs_zero_memory(void* block, u64 size);

HS_API void* hs_copy_memory(void* dest, const void* source, u64 size);

HS_API void* hs_set_memory(void* dest, i32 value, u64 size);

HS_API char* get_memory_usage();