#include "http_server.h"
#include "snail/log.h"

namespace snail
{
    namespace http
    {
        static snail::Logger::ptr g_logger = SNAIL_LOG_NAME("system");

        HttpServer::HttpServer(bool keepalive, snail::IOManager* worker, snail::IOManager* accept_worker)
            :TcpServer(worker, accept_worker)
            , m_isKeepalive(keepalive)
        {
            m_dispatch.reset(new ServletDispatch());
        }

        void HttpServer::handleClient(Socket::ptr client)
        {
            HttpSession::ptr session(new HttpSession(client));
            do 
            {   
                auto req = session->recvRequest();
                if(!req)
                {
                    SNAIL_LOG_ERROR(g_logger) << "recv http request failed, errno="
                        << errno << " errstr=" << strerror(errno)
                        << " client=" << *client;
                    break;
                }


                HttpResponse::ptr rsp(new HttpResponse(req->getVersion(), req->isClose() || !m_isKeepalive));

                m_dispatch->handle(req, rsp, session);
                // rsp->setBody("hello snail");
                // SNAIL_LOG_INFO(g_logger) << "request: " << std::endl
                //     << *req;
                // SNAIL_LOG_INFO(g_logger) << "response: " << std::endl
                //     << *rsp;
                session->sendResponse(rsp);
            }while(m_isKeepalive);
        }
    }
}