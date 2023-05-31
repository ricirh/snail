#ifndef __SNAIL_TCP_SERVER__
#define __SNAIL_TCP_SERVER__

#include <memory>
#include <functional>
#include <vector>
#include "iomanager.h"
#include "socket.h"
#include "noncopyable.h"
#include "noncpoyable.h"

namespace snail
{
    class TcpServer : public std::enable_shared_from_this<TcpServer>
        , Noncopyable
    {
    public:
        typedef std::shared_ptr<TcpServer> ptr;
        TcpServer(snail::IOManager *worker = snail::IOManager::GetThis()
                , snail::IOManager *acceptWorker = snail::IOManager::GetThis());
        virtual  ~TcpServer();

        virtual bool bind(snail::Address::ptr addr);
        virtual bool bind(const std::vector<Address::ptr> &addrs
                        , std::vector<Address::ptr> &fails);
        virtual bool start();
        virtual void stop();

        uint64_t getRecvTimeout() const { return m_recvTimeout; }
        std::string getName() const { return m_name; }
        void setName(const std::string &name) { m_name = name; }
        void setRecvTimeout(uint64_t v) { m_recvTimeout = v; }

        bool isStop() const { return m_isStop; }

    protected:
        virtual void handleClient(Socket::ptr client);
        virtual void startAccept(Socket::ptr sock);

    private:
        std::vector<Socket::ptr> m_socks;
        IOManager *m_worker;
        IOManager *m_acceptWorker;
        uint64_t m_recvTimeout;
        std::string m_name;
        bool m_isStop;
    };
    
}


#endif