#ifndef __SNAIL_HTTP_PARSER_H__
#define  __SNAIL_HTTP_PARSER_H__

#include "http.h"
#include "http11_parser.h"
#include "httpclient_parser.h"

namespace snail
{
    namespace http
    {
        class HttpRequestParser
        {
        public:
            typedef std::shared_ptr<HttpRequestParser> ptr;
            HttpRequestParser();

            int isFinished();
            int hasError();
            size_t execute(char* data, size_t len);

            HttpRequest::ptr getData() const { return m_data; }
            void setError(int error) { m_error = error; }

            uint64_t getContentLength();
            const http_parser& getParser() const { return m_parser; }
        public:
            
            static uint64_t GetHttpRequestBufferSize();
            static uint64_t GetHttpRequestMaxBodySize();
        private:
            http_parser m_parser;
            HttpRequest::ptr m_data;
            int m_error;
        };

        class HttpResponseParser
        {
        public:
            typedef std::shared_ptr<HttpResponseParser> ptr;
            HttpResponseParser();

            int isFinished();
            int hasError();
            size_t execute(char* data, size_t len, bool chunck = false);

            HttpResponse::ptr getData() const { return m_data; }

            void setError(int error) { m_error = error; }

            uint64_t getContentLength();
            const httpclient_parser& getParser() const { return m_parser; }
        public:
            static uint64_t GetHttpResponseBufferSize();
            static uint64_t GetHttpResponseMaxBodySize();
        private:
            httpclient_parser m_parser;
            HttpResponse::ptr m_data;
            int m_error;
        };
    }
}


#endif