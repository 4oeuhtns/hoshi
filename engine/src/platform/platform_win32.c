#include "platform/platform.h"

// Windows platform implementation
#if PLATFORM_WINDOWS

#include <windows.h>
#include <windowsx.h>

#include "core/logger.h"

// Internal state for windows platform
typedef struct internal_state {
    HINSTANCE instance; // Instance of the application
    HWND window; // Window handle
} internal_state;

LRESULT CALLBACK win32_process_message(HWND window, u32 message, WPARAM w_param, LPARAM l_param); /* {
    LRESULT result = 0;
    switch (message) {
        case WM_CLOSE: {
            DestroyWindow(window);
        } break;
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        default: {
            result = DefWindowProc(window, message, w_param, l_param);
        } break;
    }
    return result;
}
*/

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
    
    // Get instance from state
    state->instance = GetModuleHandle(0);

    // Setup window class
    HICON icon = LoadIcon(state->instance, IDI_APPLICATION);
    WNDCLASSEXA wc = {0};
    wc.style = CS_DBLCLKS; // Double click messages
    wc.lpfnWndProc = win32_process_message;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->instance;
    wc.hIcon = icon;
    wc.hIconSm = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Cursor managed manually
    wc.hbrBackground = NULL; // No background
    wc.lpszClassName = "HSWindowClass";

    // Register window class
    if (!RegisterClassExA(&wc)) {
        MessageBoxA(0, "Failed to register window class.", "Error", MB_OK | MB_ICONERROR);
        HS_FATAL("Failed to register window class.");
        return FALSE;
    }

    // Create window
    // Size of actual client window area
    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;
    
    // Size of window including borders
    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    // Window style
    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
    u32 window_ex_style = WS_EX_APPWINDOW;

    // Get size of border
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // Calculate window size
    window_x += border_rect.left;
    window_y += border_rect.top;
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA(
        window_ex_style,
        wc.lpszClassName,
        window_title,
        window_style,
        window_x,
        window_y,
        window_width,
        window_height,
        0,
        0,
        state->instance,
        0
    );

    // Checks if window creation failed
    if (!handle) {
        MessageBoxA(0, "Failed to create window.", "Error", MB_OK | MB_ICONERROR);
        HS_FATAL("Failed to create window.");
        return FALSE;
    } else {
        state->window = handle;
    }

    // Show window
    i32 show_cmd = SW_SHOW;
    // initially minimized, SW_MINIMIZE
    // initially maximized, SW_SHOWMAXIMIZED
    ShowWindow(state->window, show_cmd);

    return TRUE;
}

void shutdown_platform(platform_state* plat_state) {
    internal_state *state = (internal_state*)plat_state->internal_state;

    if (state->window) {
        DestroyWindow(state->window);
        state->window = 0;
    }
}

b8 platform_pump_messages(platform_state* plat_state) {
    
}

#endif