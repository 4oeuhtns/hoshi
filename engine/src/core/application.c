#include "application.h"
#include "game_types.h"

#include "logger.h"
#include "platform/platform.h"

// Structure representing the state of the application
typedef struct application_state {
    game* game_instance;
    b8 is_running;
    b8 is_suspended;
    platform_state platform; // platform contains internal state of platform
    i16 width;
    i16 height;
    f64 last_time;
} application_state;
static b8 initialized = FALSE;

// Static instance of the application state
static application_state app_state;

HS_API b8 init_application(game* game_instance) {
    if (initialized) {
        HS_ERROR("Application already initialized!");
        return FALSE;
    }
    app_state.game_instance = game_instance;

    // Initialize subsystems
    init_logger();

    // TODO: remove
    HS_FATAL("Hello, fatal! %d", 45621);
    HS_ERROR("Hello, error!");
    HS_WARN("Hello, warn!");
    HS_DEBUG("Hello, debug!");
    HS_INFO("Hello, info!");
    HS_TRACE("Hello, trace!");

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    // Game instance stores config, managed by application
    if (!init_platform(
        &app_state.platform,
        game_instance->app_config.title,
        game_instance->app_config.start_x,
        game_instance->app_config.start_y,
        game_instance->app_config.width,
        game_instance->app_config.height)) {
        HS_ERROR("Failed to initialize platform!");
        return FALSE;
    }

    // Initialize game
    if (!app_state.game_instance->init(app_state.game_instance)) {
        HS_FATAL("Failed to initialize game!");
        return FALSE;
    }

    // Handle resizing of application
    app_state.game_instance->on_resize(app_state.game_instance, app_state.width, app_state.height);

    initialized = TRUE;
    return TRUE;
}

HS_API b8 run_application() {
    while(app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            // Game update
            if (!app_state.game_instance->update(app_state.game_instance, (f64)0)) {
                HS_FATAL("Game update failed! Shutting down...");
                app_state.is_running = FALSE;
                break;
            }

            // Game render
            if (!app_state.game_instance->render(app_state.game_instance, (f64)0)) {
                HS_FATAL("Game rendering failed! Shutting down...");
                app_state.is_running = FALSE;
                break;
            }
        }
    }
    app_state.is_running = FALSE;
    shutdown_platform(&app_state.platform);
    return TRUE;
}