#pragma once

#include "defines.h"

// Pointer to state of platform, type determined by implementation
typedef struct platform_state {
    void* internal_state;
} platform_state;

// ---------- Methods ----------
b8 init_platform(
    platform_state* plat_state,
    const char* window_title,
    i32 x,
    i32 y,
    i32 width,
    i32 height
);
void shutdown_platform(platform_state* plat_state);

// Platform update methods
b8 platform_pump_messages(platform_state* plat_state);

// Memory allocation methods
void* platform_alloc(u64 size, b8 aligned);
void platform_free(void* block, b8 aligned);
void* platform_zero_memory(void* block, u64 size);
void* platform_copy_memory(void* dest, const void* source, u64 size);
void* platform_set_memory(void* dest, i32 value, u64 size);

// Console output depending on platform
void platform_console_write(const char* message, u8 color);
void platform_console_write_error(const char* message, u8 color);

// Time methods
f64 platform_get_time();
void platform_sleep(u64 ms);
// -----------------------------