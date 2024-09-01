#pragma once

#include "core/application.h"

// Basic game state
typedef struct game {
    // Application configuration
    application_config app_config;

    // Function pointer to game initialization
    b8 (*init)(struct game* game_instance);

    // Function pointer to game update
    b8 (*update)(struct game* game_instance, f64 delta_time);

    // Function pointer to game rendering
    b8 (*render)(struct game* game_instance, f64 delta_time);

    // Function pointer to handle resizing
    void (*on_resize)(struct game* game_instance, u32 width, u32 height);

    // Game specific game state, managed by game
    void* game_state;
} game;