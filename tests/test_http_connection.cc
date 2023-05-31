#include <iostream>
#include "../snail/http/http_connection.h"
#include "../snail/log.h"
#include "../snail/iomanager.h"

static snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void test_pool()
{
    snail::http::HttpConnectionPool::ptr pool(new snail::http::HttpConnectionPool(
        "www.sylar.top", "", 80, 10, 1000 * 30, 5));
    
    snail::IOManager::GetThis()->addTimer(1000, [pool](){
        auto r = pool->doGet("/", 300);
        SNAIL_LOG_INFO(g_logger) << r->toString();
    }, true);

}

void run()
{
    snail::Address::ptr addr = snail::Address::LookupAny("www.baidu.com:80");
    if(!addr)
    {
        SNAIL_LOG_ERROR(g_logger) << "get addr fail";
        return ;
    }

    snail::Socket::ptr sock = snail::Socket::CreateTCP(addr);
    bool rt = sock->connect(addr);
    if(!rt)
    {
        SNAIL_LOG_ERROR(g_logger) << "connect " << *addr << " failed";
        return ;
    }

    snail::http::HttpConnection::ptr con(new snail::http::HttpConnection(sock));
    snail::http::HttpRequest::ptr req(new snail::http::HttpRequest);
    SNAIL_LOG_INFO(g_logger) << "request:" << std::endl << *req;

    con->sendRequest(req);
    auto rsp = con->recvResponse();

    if(!rsp)
    {
        SNAIL_LOG_ERROR(g_logger) << "recv response failed";
        return;
    }

    SNAIL_LOG_INFO(g_logger) << "response:" << std::endl << *rsp;


    SNAIL_LOG_INFO(g_logger) << "=======================================";
    auto r = snail::http::HttpConnection::DoGet("http://www.baidu.com", 300);
    SNAIL_LOG_INFO(g_logger) << "result=" << r->result
        << "error=" << r->error
        << " rsp=" << ( r->response ? r->response->toString() : "");

    test_pool();

}


int main(int argc, char *argv[])
{
    snail::IOManager iom(2);
    iom.schedule(run);
    return 0;
}

