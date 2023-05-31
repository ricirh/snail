#include "../snail/snail.h"
#include <assert.h>

snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void test_assert()
{
    SNAIL_LOG_INFO(g_logger) << snail::BacktraceToString(10);

    SNAIL_ASSERT(false);
}


int main(int argc, char** argv)
{
    test_assert();
    return 0;
}
