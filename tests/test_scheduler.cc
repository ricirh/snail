#include "../snail/snail.h"

static snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void test_fiber()
{
    static int s_count = 5;
    SNAIL_LOG_INFO(g_logger) << "test in fiber" << s_count;
    
    sleep(1);
    if(--s_count >= 0)
    {
       snail::Scheduler::GetThis()->schedule(&test_fiber, snail::GetThreadId());
    }
}

int main(int argc, char** argv)
{
    snail::Scheduler sc(3, false, "test");
    sc.start();
    sc.schedule(&test_fiber);
    sc.stop();
    SNAIL_LOG_INFO(g_logger) << "over";
    return 0;
}