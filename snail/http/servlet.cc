#include <fnmatch.h>

#include "servlet.h"

namespace snail
{
    namespace http 
    {
        FunctionServlet::FunctionServlet(callback cb)
            : Servlet("FunctionServlet")
            , m_cb(cb)
        {

        }

        int32_t FunctionServlet::handle(snail::http::HttpRequest::ptr request
            , snail::http::HttpResponse::ptr response
            , snail::http::HttpSession::ptr session)
        {
            return m_cb(request, response, session);
        }

        ServletDispatch::ServletDispatch()
            :Servlet("ServletDispatch")
            ,m_default(new NotFundServlet())
        {

        }

        int32_t ServletDispatch::handle(snail::http::HttpRequest::ptr request
            , snail::http::HttpResponse::ptr response
            , snail::http::HttpSession::ptr session)
        {
            auto slt = getMatchedServlet(request->getPath());
            if(slt)
            {
                return slt->handle(request, response, session);
            }
        }
        
        void ServletDispatch::addServlet(const std::string& uri, Servlet::ptr slt)
        {
            MutexType::WriteLock lock(m_mutex);
            m_datas[uri] = slt;
        }

        void ServletDispatch::addServlet(const std::string& uri, FunctionServlet::callback cb)
        {
            MutexType::WriteLock lock(m_mutex);
            m_datas[uri].reset(new FunctionServlet(cb));
        }


        void ServletDispatch::addGlobServlet(const std::string& uri, Servlet::ptr slt)
        {
            MutexType::WriteLock lock(m_mutex);
            for (auto it = m_globs.begin(); it != m_globs.end(); ++it)
            {
                if(it->first == uri)
                {
                    m_globs.erase(it);
                    break;
                }
            }
            m_globs.push_back(std::make_pair(uri, slt));
        }

        void ServletDispatch::addGlobServlet(const std::string& uri, FunctionServlet::callback cb)
        {
            return addGlobServlet(uri, FunctionServlet::ptr(new FunctionServlet(cb)));
        }


        void ServletDispatch::delServlet(const std::string& uri)
        {
            MutexType::WriteLock lock(m_mutex);
            m_datas.erase(uri);
        }

        void ServletDispatch::delGlobServlet(const std::string& uri)
        {
            MutexType::WriteLock lock(m_mutex);
            for (auto it = m_globs.begin(); it != m_globs.end(); ++it)
            {
                if(it->first == uri)
                {
                    m_globs.erase(it);
                    break;
                }
            }
        }

        Servlet::ptr ServletDispatch::getServlet(const std::string& uri)
        {
            MutexType::ReadLock lock(m_mutex);
            auto it = m_datas.find(uri);
            return it == m_datas.end() ? nullptr : it->second;
        }

        Servlet::ptr ServletDispatch::getGlobServlet(const std::string& uri)
        {
            MutexType::ReadLock lock(m_mutex);
            for (auto it = m_globs.begin(); it != m_globs.end(); ++it)
            {
                if(it->first == uri)
                {
                    return it->second;
                }
            }
            return nullptr;
        }

        Servlet::ptr ServletDispatch::getMatchedServlet(const std::string& uri)
        {
            MutexType::ReadLock lock(m_mutex);
            auto mit = m_datas.find(uri);
            if(mit != m_datas.end())
            {
                return mit->second;
            }

            for (auto it = m_globs.begin(); it != m_globs.end(); ++it)
            {
                if(!fnmatch(it->first.c_str(), uri.c_str(), 0))
                {
                    return it->second;
                }
            }
            return m_default;
        }


        NotFundServlet::NotFundServlet()
            :Servlet("NotFundServlet")
        {

        }

        int32_t NotFundServlet::handle(snail::http::HttpRequest::ptr request
                , snail::http::HttpResponse::ptr response
                , snail::http::HttpSession::ptr session)
        {
            static const std::string& RSP_BODY = "<html><head><title>404 Not Found</title></head>"
                "<body><h1>404 Not Found</h1>"
                "<p>The requested URL was not found on this server.</p>"
                "</body></html>";
            
            response->setHeader("Content-Type", "text/html");
            response->setHeader("Server", "snail/1.0");
            response->setStatus(snail::http::HttpStatus::NOT_FOUND);
            response->setBody(RSP_BODY);
            return 0;
        }
    }
}