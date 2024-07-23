#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1


// Disable logging in release mode
#if HS_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

// Log levels
typedef enum LogLevel {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} LogLevel;

// ---------- Methods ----------
b8 init_logger();
void shutdown_logger();

HS_API void log_message(LogLevel level, const char* message, ...);
// -----------------------------

// Fatal level log message
#define HS_FATAL(message, ...) log_message(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

// Error level log message
#define HS_ERROR(message, ...) log_message(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)

// Warn level log message
#if LOG_WARN_ENABLED == 1
#define HS_WARN(message, ...) log_message(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
// If warn level is disabled, define it as an empty macro
#define HS_WARN(message, ...)
#endif

// Info level log message
#if LOG_INFO_ENABLED == 1
#define HS_INFO(message, ...) log_message(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
// If info level is disabled, define it as an empty macro
#define HS_INFO(message, ...)
#endif

// Debug level log message
#if LOG_DEBUG_ENABLED == 1
#define HS_DEBUG(message, ...) log_message(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
// If debug level is disabled, define it as an empty macro
#define HS_DEBUG(message, ...)
#endif

// Trace level log message
#if LOG_TRACE_ENABLED == 1
#define HS_TRACE(message, ...) log_message(LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
// If trace level is disabled, define it as an empty macro
#define HS_TRACE(message, ...)
#endif