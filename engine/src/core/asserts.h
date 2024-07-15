#pragma once

#include "defines.h"

// Comment to disable assertions
#define HS_ASSERTIONS_ENABLED

#ifdef HS_ASSERTIONS_ENABLED
// Defines debug break based on platform
#if _MSC_VER
#include <intrin.h>
#define debug_break() __debugbreak()
#else
#define debug_break() __builtin_trap()
#endif

// ---------- Methods ----------
HS_API void assert_handler(const char* expression, const char* message, const char* file, u32 line);
// -----------------------------

// Assert macros
#define HS_ASSERT(expr) {                              \
    if (expr) {                                        \
    } else {                                           \
        assert_handler(#expr, "", __FILE__, __LINE__); \
        debug_break();                                 \
    }                                                  \
}                                                      \

#define HS_ASSERT_MSG(expr, msg) {                      \
    if (expr) {                                         \
    } else {                                            \
        assert_handler(#expr, msg, __FILE__, __LINE__); \
        debug_break();                                  \
    }                                                   \
}                                                       \

#ifdef _DEBUG
#define HS_ASSERT_DEBUG(expr) {                        \
    if (expr) {                                        \
    } else {                                           \
        assert_handler(#expr, "", __FILE__, __LINE__); \
        debug_break();                                 \
    }                                                  \
}                                                      \

#else
//Does nothing in release mode
#define HS_ASSERT_DEBUG(expr)
#endif

// Does nothing if assertions are disabled
#else
#define HS_ASSERT(expr)
#define HS_ASSERT_MSG(expr, msg)
#define HS_ASSERT_DEBUG(expr)

#endif