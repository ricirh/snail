#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "../snail/snail.h"
#include "../snail/iomanager.h"

snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void test_fiber()
{
    SNAIL_LOG_INFO(g_logger) << "test in fiber";
}

void test1()
{
    snail::IOManager iom;
    iom.schedule(&test_fiber);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);
    
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_pton(AF_INET, "36.152.44.95", &addr.sin_addr.s_addr);

    

    if(!connect(sock, (const sockaddr*)&addr, sizeof(addr)))
    {

    }
    else if(errno == EINPROGRESS)
    {
        snail::IOManager::GetThis()->addEvent(sock, snail::IOManager::READ, []()
        {
            SNAIL_LOG_INFO(g_logger) << "read callback";
        });

        snail::IOManager::GetThis()->addEvent(sock, snail::IOManager::WRITE, [=]()
        {
            SNAIL_LOG_INFO(g_logger) << "write callback";
            snail::IOManager::GetThis()->cancelEvent(sock, snail::IOManager::READ);
            close(sock);
        });
    }
}

snail::Timer::ptr timer;

void test_timer()
{
    snail::IOManager iom(2);

    timer = iom.addTimer(500, [timer]()
        {
            static int i = 0;
            SNAIL_LOG_INFO(g_logger) << "time out i = " << i;
            if(++i == 5)
            {
                //timer->cancel();
                timer->reset(2000, true);
            }
        }
    , true);
}

int main(int argc, char** argv)
{
    test1();
    test_timer();
    return 0;
}