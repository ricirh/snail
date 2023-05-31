#include "../snail/http/http.h"
#include "../snail/log.h"

void test_request()
{
    snail::http::HttpRequest::ptr req(new snail::http::HttpRequest);
    req->setHeader("host", "www.baidu.com");
    req->setBody("hello world");
    req->dump(std::cout) << std::endl;
}

void test_resonse()
{
    snail::http::HttpResponse::ptr res(new snail::http::HttpResponse);
    res->setHeader("XX", "snail");
    res->setBody("hello snail");
    res->setStatus((snail::http::HttpStatus)400);
    res->setClose(false);
    res->dump(std::cout) << std::endl;
}

int main(int argc, char **argv)
{
    test_request();
    test_resonse();
    return 0;
}