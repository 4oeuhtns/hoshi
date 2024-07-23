#include "platform/platform.h"

// Windows platform implementation
#if PLATFORM_WINDOWS

#include "core/logger.h"

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

// Internal state for windows platform
typedef struct internal_state {
    HINSTANCE instance; // Instance of the application
    HWND window; // Window handle
} internal_state;

// Clock
static f64 clock_frequency;
static LARGE_INTEGER clock_start;


LRESULT CALLBACK win32_process_message(HWND window, u32 message, WPARAM w_param, LPARAM l_param);

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

    // Clock setup
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&clock_start);

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
    MSG message;
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return TRUE;
}

void* platform_alloc(u64 size, b8 aligned) {
    return malloc(size); //TODO: alinged memory allocation
}

void platform_free(void* block, b8 aligned) {
    free(block); //TODO: alinged memory allocation
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
    // Console color
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    static u8 color_levels[6] = {
        FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE, // fatal, magenta
        FOREGROUND_INTENSITY | FOREGROUND_RED, // error, red
        FOREGROUND_RED | FOREGROUND_GREEN, // warn, yellow
        FOREGROUND_GREEN, // info, green
        FOREGROUND_BLUE, // debug, blue
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE // trace, gray
    };
    SetConsoleTextAttribute(console_handle, color_levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD written = 0;
    WriteConsoleA(console_handle, message, (DWORD)length, written, 0);
}

void platform_console_write_error(const char* message, u8 color) {
    // Console color
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    static u8 color_levels[6] = {
        FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE, // fatal, magenta
        FOREGROUND_INTENSITY | FOREGROUND_RED, // error, red
        FOREGROUND_RED | FOREGROUND_GREEN, // warn, yellow
        FOREGROUND_GREEN, // info, green
        FOREGROUND_BLUE, // debug, blue
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE // trace, gray
    };
    SetConsoleTextAttribute(console_handle, color_levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD written = 0;
    WriteConsoleA(console_handle, message, (DWORD)length, written, 0);
}

// FIXME: platform time might be wrong, in video, did not subtract start time
f64 platform_get_time() {
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    return (f64)(current_time.QuadPart - clock_start.QuadPart) * clock_frequency;
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}

// Window message processing
LRESULT CALLBACK win32_process_message(HWND window, u32 message, WPARAM w_param, LPARAM l_param) {
    switch (message) {
        case WM_ERASEBKGND:
            return 1; // erase handled by application
        case WM_CLOSE:
            return 0; // TODO: fire event for application to quit
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // Get updated window size
            // RECT rect; 
            // GetClientRect(window, &rect);
            // u32 width = rect.right - rect.left;
            // u32 height = rect.bottom - rect.top;

            // TODO: fire event for application to resize
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // Pressed or released
            // b8 pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
            // TODO: input processing

        } break;
        case WM_MOUSEMOVE: {
            // Mouse move
            // i32 x = GET_X_LPARAM(l_param);
            // i32 y = GET_Y_LPARAM(l_param);
            // TODO: input processing
        } break;
        case WM_MOUSEWHEEL: {
            // i32 delta = GET_WHEEL_DELTA_WPARAM(w_param);
            // if (delta != 0) {
            //     delta = (delta > 0) ? 1 : -1;
            // }
            // TODO: input processing
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            // b8 pressed = (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN);
            // TODO: input processing
        } break;
        default:
            return DefWindowProcA(window, message, w_param, l_param);
    }
}

#endif