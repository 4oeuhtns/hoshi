#pragma once

#include "defines.h"

// Forward declaration
struct game;

// Application configuration
typedef struct application_config {
    const char* title;
    i16 start_x;
    i16 start_y;
    i16 width;
    i16 height;
} application_config;

// ---------- Methods ----------
HS_API b8 init_application(struct game* game_instance);

HS_API b8 run_application();