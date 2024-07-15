#include "asserts.h"
#include "logger.h"

void assert_handler(const char* expression, const char* message, const char* file, u32 line) {
    if (message[0] != '\0') {
        // Message is not empty, include it in the output
        HS_FATAL("Assertion failed: %s\n         %s\n         File: %s\n         Line: %d", expression, message, file, line);
    } else {
        // Message is empty, exclude it from the output
        HS_FATAL("Assertion failed: %s\n%s         File: %s\n         Line: %d", expression, message, file, line);
    }
}