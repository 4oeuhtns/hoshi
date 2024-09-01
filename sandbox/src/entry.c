#include "game.h"

#include <entry.h>

// TODO: temporary, remove
#include <platform/platform.h>

// Define function to create game
b8 create_game(game* out_game) {

    // Application configuration
    out_game->app_config.start_x = 0;
    out_game->app_config.start_y = 0;
    out_game->app_config.width = 1280;
    out_game->app_config.height = 720;
    out_game->app_config.title = "Sandbox";
    out_game->init = game_init;
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->on_resize = game_on_resize;
    
    // Create game state
    out_game->game_state = platform_alloc(sizeof(game_state), FALSE);

    return TRUE;
}