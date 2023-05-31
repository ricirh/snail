#include "../snail/address.h"
#include "../snail/log.h"


snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void test()
{
    std::vector<snail::Address::ptr> addrs;
    SNAIL_LOG_INFO(g_logger) << "begin";
    bool v = snail::Address::Lookup(addrs, "www.baidu.com");
    SNAIL_LOG_INFO(g_logger) << "end";
    if(!v)
    {
        SNAIL_LOG_ERROR(g_logger) << "lookup fail";
        return ;
    }
    for(size_t i = 0; i < addrs.size(); ++i)
    {
        SNAIL_LOG_INFO(g_logger) << i << " - " << addrs[i]->toString();
    }
}

void test_iface()
{
    std::multimap<std::string, std::pair<snail::Address::ptr, uint32_t> > results;
    bool v = snail::Address::GetInterfaceAddresses(results);
    if(!v)
    {
        SNAIL_LOG_ERROR(g_logger) << "GetInterfaceAddresses fail";
        return ;
    }

    for(auto &i : results)
    {
        SNAIL_LOG_INFO(g_logger) << i.first << " - " << i.second.first->toString() << " - "
            << i.second.second;
    }

}

void test_ipv4()
{
    auto addr = snail::IPAddress::Create("www.baidu.com");
    if(addr)
    {
        SNAIL_LOG_INFO(g_logger) << addr->toString();
    }
}

int main(int argc, char** argv)
{
    //test();
    //test_iface();
    test_ipv4();
    return 0;
}