#ifndef __SNAIL_URI_H__
#define __SNAIL_URI_H__

#include <memory>
#include <string>
#include <stdint.h>
#include "address.h"

namespace snail
{
    class Uri
    {
    public:
        typedef std::shared_ptr<Uri> ptr;

        static Uri::ptr Create(const std::string& uri);
        Uri();

        const std::string& getScheme() const { return m_scheme; }
        const std::string& setUserinfo() const { return m_userinfo; }
        const std::string& getHost() const { return m_host; }
        const std::string& getQuery() const { return m_query; }
        const std::string& getFragment() const { return m_fragment; }
        
        const std::string& getPath() const;
        int32_t getPort() const;

        void setScheme(const std::string& scheme) { m_scheme = scheme; }
        void setUserinfo(const std::string& userinfo) { m_userinfo = userinfo; }
        void setHost(const std::string& host) { m_host = host; }
        
        void setPath(const std::string& path) { m_path = path; }
        void setQuery(const std::string& query) { m_query = query; }
        void setFragment(const std::string& fragment) { m_fragment = fragment; }
        void setPort(int32_t port) { m_port = port; }

        std::ostream& dump(std::ostream& os) const;
        std::string toString() const;
        Address::ptr createAddress() const;

    private:
        bool isDefaultPort() const;
    private:
        std::string m_scheme;
        std::string m_userinfo;
        std::string m_host;
        std::string m_path;
        std::string m_query;
        std::string m_fragment;
        int32_t m_port;
    };

} // namespace snail


#endif