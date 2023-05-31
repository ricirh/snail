#ifndef __SNAIL_HTTP_SERVER_H__
#define __SNAIL_HTTP_SERVER_H__

#include "snail/tcp_server.h"
#include "http_session.h"
#include "servlet.h"

namespace snail
{
    namespace http
    {
        class HttpServer : public TcpServer
        {
        public:
            typedef std::shared_ptr<HttpServer> ptr;
            HttpServer(bool keepalive = false
            , snail::IOManager* worker = snail::IOManager::GetThis()
            , snail::IOManager* accept_worker = snail::IOManager::GetThis());

            ServletDispatch::ptr getServletDispatch() const { return m_dispatch; }
            void setServletDispatch(ServletDispatch::ptr v) { m_dispatch = v; }

        protected:
            virtual void handleClient(Socket::ptr client) override;
        private:
            bool m_isKeepalive;
            ServletDispatch::ptr m_dispatch;
        };
    }
}


#endif