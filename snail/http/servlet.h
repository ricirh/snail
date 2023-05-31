#ifndef __SNAIL_HTTP_SERVLET_H__
#define __SNAIL_HTTP_SERVLET_H__

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include "http.h"
#include "http_session.h"
#include "snail/thread.h"
namespace snail
{
    namespace http
    {
        class Servlet
        {
        public:
            typedef std::shared_ptr<Servlet> ptr;

            Servlet(const std::string &name)
                :m_name(name) { }
            virtual ~Servlet() {}
            virtual int32_t handle(snail::http::HttpRequest::ptr request
                , snail::http::HttpResponse::ptr response
                , snail::http::HttpSession::ptr session) = 0;
            
            const std::string &name() const { return m_name; }
        protected:
            std::string m_name;
        };

        class FunctionServlet : public Servlet
        {
        public:
            typedef std::shared_ptr<FunctionServlet> ptr;
            typedef std::function<int32_t(snail::http::HttpRequest::ptr request
                , snail::http::HttpResponse::ptr response
                , snail::http::HttpSession::ptr session)> callback;

            FunctionServlet(callback cb);
            virtual int32_t handle(snail::http::HttpRequest::ptr request
                , snail::http::HttpResponse::ptr response
                , snail::http::HttpSession::ptr session) override;
        
        private:
            callback m_cb;
        };

        class ServletDispatch : public Servlet
        {
        public:
            typedef std::shared_ptr<ServletDispatch> ptr;
            typedef RWMutex MutexType;


            ServletDispatch();
            virtual int32_t handle(snail::http::HttpRequest::ptr request
                , snail::http::HttpResponse::ptr response
                , snail::http::HttpSession::ptr session) override;
            
            void addServlet(const std::string& uri, Servlet::ptr slt);
            void addServlet(const std::string& uri, FunctionServlet::callback cb);

            void addGlobServlet(const std::string& uri, Servlet::ptr slt);
            void addGlobServlet(const std::string& uri, FunctionServlet::callback cb);

            void delServlet(const std::string& uri);
            void delGlobServlet(const std::string& uri);

            Servlet::ptr getServlet(const std::string& uri);
            Servlet::ptr getGlobServlet(const std::string& uri);
            Servlet::ptr getMatchedServlet(const std::string& uri);

            Servlet::ptr getDefaultServlet() { return m_default; }
            void setDefaultServlet(Servlet::ptr slt) { m_default = slt; }


        private:
            MutexType m_mutex;
            std::unordered_map<std::string, Servlet::ptr> m_datas;
            std::vector<std::pair<std::string, Servlet::ptr>> m_globs;

            Servlet::ptr m_default;
        };

        class NotFundServlet : public Servlet
        {
        public:
            typedef std::shared_ptr<NotFundServlet> ptr;
            NotFundServlet();
            virtual int32_t handle(snail::http::HttpRequest::ptr request
                , snail::http::HttpResponse::ptr response
                , snail::http::HttpSession::ptr session) override;
        };
    }
}

#endif