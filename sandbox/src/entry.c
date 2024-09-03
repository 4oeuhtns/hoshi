#include "game.h"

#include <entry.h>

#include <core/hs_memory.h>

// Define function to create game
b8 create_game(game* out_game) {

    // Application configuration
    out_game->app_config.title = "Sandbox";
    out_game->app_config.start_x = 0;
    out_game->app_config.start_y = 0;
    out_game->app_config.width = 1280;
    out_game->app_config.height = 720;

    // Set function pointers
    out_game->init = game_init;
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->on_resize = game_on_resize;
    
    // Create game state
    out_game->game_state = hs_alloc(sizeof(game_state), MEMORY_TAG_GAME);

    return TRUE;
}