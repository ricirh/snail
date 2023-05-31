#include "../snail/socket.h"
#include "../snail/log.h"
#include "../snail/iomanager.h"

static snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void test_socket()
{
    snail::IPAddress::ptr addr = snail::Address::LookupAnyIPAddress("www.baidu.com");
    if(addr)
    {
        SNAIL_LOG_INFO(g_logger) << "get address: " << addr->toString();
    }
    else
    {
        SNAIL_LOG_ERROR(g_logger) << "get address fail";
        return;
    }
    
    snail::Socket::ptr sock = snail::Socket::CreateTCP(addr);
    addr->setPort(80);
    if(!sock->connect(addr))
    {
        SNAIL_LOG_ERROR(g_logger) << "connect " << addr->toString() << " fail";
        return;
    }
    else
    {
        SNAIL_LOG_ERROR(g_logger) << "connect " << addr->toString() << " connected";
    }

    const char buff[] = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\n\r\n";
    int rt = sock->send(buff, sizeof(buff));
    if(rt <= 0)
    {
        SNAIL_LOG_ERROR(g_logger) << "send fail rt=" << rt;
        return ;
    }

    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());
    
    if(rt <= 0)
    {
        SNAIL_LOG_INFO(g_logger) << "recv fail rt=" << rt;
        return;
    }

    buffs.resize(rt);

    SNAIL_LOG_INFO(g_logger) << buffs;
}

int main(int argc, char** argv)
{
    snail::IOManager iom;
    iom.schedule(&test_socket);
    return 0;
}