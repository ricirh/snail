#include "../snail/snail.h"

snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void run_in_fiber()
{
    SNAIL_LOG_INFO(g_logger) << "run_in_fiber begin";
    //snail::Fiber::GetThis()->swapOut();
    snail::Fiber::YieldToHold();
    SNAIL_LOG_INFO(g_logger) << "run_in fiber end";
}

void test_fiber()
{
    SNAIL_LOG_INFO(g_logger) << "main begin -1";
    {
        snail::Fiber::GetThis();
        SNAIL_LOG_INFO(g_logger) << "main begin";
        snail::Fiber::ptr fiber(new snail::Fiber(run_in_fiber));
        fiber->swapIn();
        SNAIL_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        SNAIL_LOG_INFO(g_logger) << "main after end";
    }
    SNAIL_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char** argv)
{
    snail::Thread::SetName("main");

    std::vector<snail::Thread::ptr> thrs;
    for(int i = 0; i < 3; ++i)
    {
        thrs.push_back(snail::Thread::ptr(new snail::Thread(&test_fiber, "name_" + std::to_string(i))));
    }
    for(auto i : thrs)
    {
        i->join();
    }
    return 0;
}