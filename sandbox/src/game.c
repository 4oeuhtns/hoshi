#include "game.h"

#include <core/logger.h>

b8 game_init(game *game_instance) {
    HS_DEBUG("game_init called!");
    return TRUE;
}

b8 game_update(game *game_instance, f64 delta_time) {
    //HS_DEBUG("game_update called!");
    return TRUE;
}

b8 game_render(game *game_instance, f64 delta_time) {
    //HS_DEBUG("game_render called!");
    return TRUE;
}

void game_on_resize(game *game_instance, u32 width, u32 height) {
    //HS_DEBUG("game_on_resize called!");
}