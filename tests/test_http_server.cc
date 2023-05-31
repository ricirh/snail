#include "../snail/http/http_server.h"
#include "../snail/log.h"
#include "../snail/http/servlet.h"

static snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void run()
{
    snail::http::HttpServer::ptr server(new snail::http::HttpServer);
    snail::Address::ptr addr = snail::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while(!server->bind(addr))
    {
        sleep(2);
    }

    auto sd = server->getServletDispatch();
    sd->addServlet("/snail/xx", [](snail::http::HttpRequest::ptr request
                                , snail::http::HttpResponse::ptr response
                                , snail::http::HttpSession::ptr session)
                                {
                                    response->setBody(request->toString());
                                    return 0;
                                }
    );

    sd->addGlobServlet("/snail/*", [](snail::http::HttpRequest::ptr request
                                , snail::http::HttpResponse::ptr response
                                , snail::http::HttpSession::ptr session)
                                {
                                    response->setBody("Glob:\r\n" + request->toString());
                                    return 0;
                                }
    );

    server->start();
}

int main(int argc, char** argv)
{
    snail::IOManager iom(2);
    iom.schedule(run);
    return 0;
}