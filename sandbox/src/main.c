#include <core/logger.h>
#include <core/asserts.h>

int main() {
    HS_FATAL("Hello, fatal! %d", 45621);
    HS_ERROR("Hello, error!");
    HS_WARN("Hello, warn!");
    HS_DEBUG("Hello, debug!");
    HS_INFO("Hello, info!");
    HS_TRACE("Hello, trace!");

    HS_ASSERT(1 == 0);
    HS_ASSERT_MSG(1 == 0, "1 is not equal to 0");
    return 0;
}