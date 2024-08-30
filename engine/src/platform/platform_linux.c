#include "platform.h"


// linux platform implementation
#if PLATFORM_LINUX

#include "core/logger.h"

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <sys/time.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h> // nanosleep
#else
#include <unistd.h> // usleep
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Internal state for linux platform
typedef struct internal_state {
    Display* display;
    xcb_connection_t* connection;
    xcb_window_t window;
    xcb_screen_t* screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_window;
} internal_state;

b8 init_platform(
    platform_state* plat_state,
    const char* window_title,
    i32 x,
    i32 y,
    i32 width,
    i32 height) {
    
    // Allocate memory for internal state
    plat_state->internal_state = malloc(sizeof(internal_state));
    internal_state *state = (internal_state*)plat_state->internal_state;

    // Connect to X
    state->display = XOpenDisplay(NULL);

    // Turn off key repeat
    XAutoRepeatOff(state->display);

    // Get display connection
    state->connection = XGetXCBConnection(state->display);

    if (xcb_connection_has_error(state->connection)) {
        HS_FATAL("Failed to get XCB connection.");
        return FALSE;
    }

    // Get data from X server
    const struct xcb_setup_t* setup = xcb_get_setup(state->connection);

    // Get screen
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
    i32 screen_num = 0;
    for (i32 s = screen_num; s > 0; s--) {
        xcb_screen_next(&it);
    }

    // Assign screen
    state->screen = it.data;

    // Allocate XID for window
    state->window = xcb_generate_id(state->connection);
    
    // Register event types
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    // Listen to mouse and keyboard events
    u32 event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | 
                       XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                       XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                       XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    
    // Values to be sent over to XCB
    u32 value_list[] = {state->screen->black_pixel, event_values};

    // Create window
    xcb_void_cookie_t window_cookie = xcb_create_window(
        state->connection,
        XCB_COPY_FROM_PARENT, // depth taken from parent window
        state->window, // window id
        state->screen->root, // parent window
        x, y, // x, y
        width, height, // width, height
        0, // no border
        XCB_WINDOW_CLASS_INPUT_OUTPUT, // window class
        state->screen->root_visual, // visual
        event_mask, // event mask
        value_list // event list
    );

    // Change title
    xcb_change_property (
        state->connection,
        XCB_PROP_MODE_REPLACE, // replace property
        state->window,
        XCB_ATOM_WM_NAME, // replace window name
        XCB_ATOM_STRING, // type of property
        8, // 8 bits at a time
        strlen(window_title), // length of string
        window_title // window title
    );

    // Handles window deletion events
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom (
        state->connection,
        0,
        strlen("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW"
    );
    xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom (
        state->connection,
        0,
        strlen("WM_PROTOCOLS"),
        "WM_PROTOCOLS"
    );
    xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply (
        state->connection,
        wm_delete_cookie,
        0
    );
    xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply (
        state->connection,
        wm_protocols_cookie,
        0
    );
    state->wm_delete_window = wm_delete_reply->atom;
    state->wm_protocols = wm_protocols_reply->atom;

    xcb_change_property (
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        wm_protocols_reply->atom,
        4,
        32,
        1,
        &wm_delete_reply->atom
    );

    // Map window
    xcb_map_window(state->connection, state->window);

    // Flush
    i32 stream = xcb_flush(state->connection);
    if (stream <= 0) {
        KFATAL("Failed to flush XCB connection: %d", stream);
        return FALSE;
    }

    return TRUE;
}

void shutdown_platform(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->interal_state;

    // turn back on auto repeat
    XAutoRepeatOn(state->display);

    xcb_destroy_window(state->connection, state->window);
}

b8 platform_pump_messages(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;

    xcb_generic_event_t* event;
    xcb_client_message_event_t* cm;

    b8 quit = FALSE;

    // poll for events
    while (event != 0) {
        event = xcb_poll_for_event(state->connection);
        if (event == 0) {
            break;
        }

        // handle events
        switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS:
            case XCB_KEY_RELEASE: {
                // TODO: key press/release
            } break;

            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
                // TODO: mouse button press/release
            } break;

            case XCB_MOTION_NOTIFY: {
                // TODO: mouse movement
            } break;

            case XCB_CONFIGURE_NOTIFY: {
                // TODO: window resize
            } break;
            
            case XCB_CLIENT_MESSAGE: {
                cm = (xcb_client_message_event_t*)event;

                // window close
                if (cm->data.data32[0] == state->wm_delete_window) {
                    quit = TRUE;
                }
                
            } break;
            
            default: {
                // unhandled event
            } break;
        }
        free(event); // dynamically allocated
    }
    return !quit;
}

void* platform_alloc(u64 size, b8 aligned) {
    return malloc(size);
}

void platform_free(void* block, b8 aligned) {
    free(block);
}

void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}

void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void platform_console_write(const char* message, u8 color) {
    const char* colors[] = {"0;105", "0;91", "0;33", "0;32", "0;34", "0;37"}; // magenta, red, yellow, green, blue, gray
    printf("\033[%sm%s\033[0m", colors[color], message);
}

void platform_console_write_error(const char* message, u8 color) {
    const char* colors[] = {"0;105", "0;91", "0;33", "0;32", "0;34", "0;37"}; // magenta, red, yellow, green, blue, gray
    printf("\033[%sm%s\033[0m", colors[color], message);
}

f64 platform_get_time() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec + time.tv_nsec * (1e-9);
}

void platform_sleep(u64 ms) {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec time;
    time.tv_sec = ms / 1000; // set seconds by converting
    time.tv_nsec = (ms % 1000) * 1000000; // set nanoseconds by modding to remove seconds first
    nanosleep(&time, 0);
#else
    if (ms >= 1000) {
        sleep(ms / 1000); // first sleep seconds
    }
    usleep ((ms % 1000) * 1000); // mod takes remainder and sleeps in milliseconds
#endif
}

#endif