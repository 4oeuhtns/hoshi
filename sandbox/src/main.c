#include <core/logger.h>
#include <core/asserts.h>
#include <platform/platform.h>

int main() {
    HS_FATAL("Hello, fatal! %d", 45621);
    HS_ERROR("Hello, error!");
    HS_WARN("Hello, warn!");
    HS_DEBUG("Hello, debug!");
    HS_INFO("Hello, info!");
    HS_TRACE("Hello, trace!");

    return 0;
}