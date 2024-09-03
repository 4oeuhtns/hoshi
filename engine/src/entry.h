#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "core/hs_memory.h"
#include "game_types.h"

// Externally defined function to create a game instance
extern b8 create_game(game* out_game);


// Main function
int main() {

    // Initialize memory
    init_memory();

    // Request game instance from application
    game game_instance;
    if (!create_game(&game_instance)) {
        HS_FATAL("Failed to create game!");
        return -1;
    }

    // Check if game function pointers are set
    if (!game_instance.init || !game_instance.update || !game_instance.render || !game_instance.on_resize) {
        HS_FATAL("Game function pointers not set!");
        return -2;
    }

    // Initialize application
    if (!init_application(&game_instance)) {
        HS_INFO("Application failed to create!");
        return 1;
    }
    
    // Begin game loop
    if(!run_application()) {
        HS_INFO("Application failed to shutdown properly.");
        return 2;
    }

    // Shutdown memory
    shutdown_memory();

    return 0;
}