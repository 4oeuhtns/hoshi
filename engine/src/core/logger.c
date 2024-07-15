#include "logger.h"

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

    // Buffer for message
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    // Format message
    __builtin_va_list args;
    va_start(args, message);
    vsnprintf(buffer, sizeof(buffer), message, args); // Formats based on message and args
    va_end(args);

    // Combine level and message into ouput buffer
    char output[4096];
    memset(output, 0, sizeof(output));
    strncpy(output, level_str[level], strlen(level_str[level]));
    strncat(output, buffer, strlen(buffer));
    strncat(output, "\n", 1);

    // TODO: Platform specific logging, file output
    printf("%s", output);
}