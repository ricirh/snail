#include "../snail/tcp_server.h"
#include "../snail/iomanager.h"
#include "../snail/log.h"


static snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void run()
{
    auto addr = snail::IPAddress::LookupAny("0.0.0.0:8081");
    //auto addr2 = snail::UnixAddress::ptr(new snail::UnixAddress("/tmp/unix_socket"));
    SNAIL_LOG_INFO(g_logger) << *addr;

    std::vector<snail::Address::ptr> addrs;
    addrs.push_back(addr);
    //addrs.push_back(addr2);

    snail::TcpServer::ptr tcp_server(new snail::TcpServer);
    std::vector<snail::Address::ptr> fails;
    while(!tcp_server->bind(addrs, fails))
    {
        sleep(2);
    }
    tcp_server->start();
}

int main(int argc, char *argv[])
{
    snail::IOManager iom(2);
    iom.schedule(run);
    return 0;
}