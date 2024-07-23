#include "logger.h"
#include "platform/platform.h"

//TODO: temp includes
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 init_logger() {
    //TODO: create log file and log in terminal
    return TRUE;
}

void shutdown_logger() {
    //TODO: cleanup logging/write queued entries
}

void log_message(LogLevel level, const char* message, ...) {
    const char* level_str[6] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARN]:  ",
        "[INFO]:  ",
        "[DEBUG]: ",
        "[TRACE]: "
    };
    b8 is_error = level < LOG_LEVEL_WARN;
    const i32 msg_length = 4096;

    // Buffer for message
    char buffer[msg_length];
    memset(buffer, 0, sizeof(buffer));

    // Format message
    __builtin_va_list args;
    va_start(args, message);
    vsnprintf(buffer, sizeof(buffer), message, args); // Formats based on message and args
    va_end(args);

    // Combine level and message into ouput buffer
    char output[msg_length];
    memset(output, 0, sizeof(output));
    strncpy(output, level_str[level], strlen(level_str[level]));
    strncat(output, buffer, strlen(buffer));
    strncat(output, "\n", 1);

    // Write to console
    if (is_error) {
        platform_console_write_error(output, level);
    } else {
        platform_console_write(output, level);
    }
}