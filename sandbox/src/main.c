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

    // platform_state state;

    // if (init_platform(&state, "Test window", 0, 0, 800, 600)) {
    //     while(TRUE) {
    //         platform_pump_messages(&state);
    //     }
    // }
    // shutdown_platform(&state);
    return 0;
}