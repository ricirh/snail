#include "../snail/http/http_parser.h"
#include "../snail/log.h"

static snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

const char test_request_data[] = "GET / HTTP/1.1\r\n"
                                "Host: www.baidu.com\r\n"
                                "Content-Length: 10\r\n\r\n"
                                "1234567890";

void test_request()
{
    snail::http::HttpRequestParser::ptr parser(new snail::http::HttpRequestParser());
    std::string tmp = test_request_data;
    size_t s = parser->execute(&tmp[0], tmp.size());
    SNAIL_LOG_INFO(g_logger) << "execute rt=" << s
         << " has_error=" << parser->hasError()
         << " is_finished=" << parser->isFinished()
         << " total=" << tmp.size()
         << " content_length=" << parser->getContentLength();

    tmp.resize(tmp.size() - s);
    SNAIL_LOG_INFO(g_logger) << parser->getData()->toString();
    SNAIL_LOG_INFO(g_logger) << tmp;
}

const char test_response_data[] = "HTTP/1.1 200 OK\r\n"
                                 "Accept-Ranges: bytes\r\n"
                                 "Connection: close\r\n"
                                 "Content-Length: 81\r\n"
                                 "Content-Type: text/html\r\n\r\n"
                                 "<html>\r\n"
                                 "<meta http-equiv=\"refresh\" content=\"0; url=http://www.baidu.com\" />\r\n"
                                 "</html>\r\n";

void test_response()
{
    snail::http::HttpResponseParser::ptr parser(new snail::http::HttpResponseParser());
    std::string tmp = test_response_data;
    size_t s = parser->execute(&tmp[0], tmp.size());

    SNAIL_LOG_INFO(g_logger) << "execute rt=" << s
         << " has_error=" << parser->hasError()
         << " is_finished=" << parser->isFinished()
         << " total=" << tmp.size()
         << " content_length=" << parser->getContentLength();
    
    tmp.resize(tmp.size() - s);
    SNAIL_LOG_INFO(g_logger) << parser->getData()->toString();
    SNAIL_LOG_INFO(g_logger) << tmp;
}

int main(int argc, char *argv[])
{
    test_request();
    SNAIL_LOG_INFO(g_logger) << "----------";
    test_response();
    return 0;
}
