#include "tcp_server.h"
#include "config.h"
#include "log.h"


namespace snail
{
    static snail::ConfigVar<uint64_t>::ptr g_tcp_server_recv_timeout =
        snail::Config::Lookup("tcp_server.recv_timeout", (uint64_t)(60 * 1000 * 2)
                , "tcp server recv timeout");

    static snail::Logger::ptr g_logger = SNAIL_LOG_NAME("system");

    TcpServer::TcpServer(snail::IOManager *worker, snail::IOManager *acceptWorker)
        :m_worker(worker)
        ,m_acceptWorker(acceptWorker)
        ,m_recvTimeout(g_tcp_server_recv_timeout->getValue())
        ,m_name("snail/1.0.0")
        ,m_isStop(true)
    {

    }

    TcpServer::~TcpServer()
    {
        for(auto &sock : m_socks)
        {
            sock->close();
        }
        m_socks.clear();
    }

    bool TcpServer::bind(snail::Address::ptr addr)
    {
        std::vector<Address::ptr> addrs;
        std::vector<Address::ptr> fails;
        addrs.push_back(addr);
        return bind(addrs, fails);
    }

    bool TcpServer::bind(const std::vector<Address::ptr> &addrs
                                , std::vector<Address::ptr> &fails)
    {
        bool rt = true;
        for(auto &addr :  addrs)
        {
            Socket::ptr sock = Socket::CreateTCP(addr);
            if(!sock->bind(addr))
            {
                SNAIL_LOG_ERROR(g_logger) << "bind fail errno=" << errno
                    << " errstr=" << strerror(errno)
                    << " addr=[" << addr->toString() << "]";
                fails.push_back(addr);
                continue;
            }
            if(!sock->listen())
            {
                SNAIL_LOG_ERROR(g_logger) << "listen fail errno=" << errno
                    << " errstr=" << strerror(errno)
                    << " addr=[" << addr->toString() << "]";
                fails.push_back(addr);
                continue;
            }
            m_socks.push_back(sock);

        }
        if(!fails.empty())
        {
            m_socks.clear();
            return true;
        }
        for(auto &i : m_socks)
        {
            SNAIL_LOG_INFO(g_logger) << "server bind success: " << *i;
        }
        return true;
    }

    bool TcpServer::start()
    {
        if(!m_isStop)
        {
            return true;
        }
        m_isStop = false;
        for(auto &sock : m_socks)
        {
            m_acceptWorker->schedule(std::bind(&TcpServer::startAccept, shared_from_this(), sock));
        }
    }

    void TcpServer::stop()
    {
        m_isStop = true;
        auto self = shared_from_this();
        m_acceptWorker->schedule([this, self](){
            for(auto &sock : m_socks)
            {
                sock->cancelAll();
                sock->close();
            }
            m_socks.clear();
        });
    }

    void TcpServer::handleClient(Socket::ptr client)
    {
        SNAIL_LOG_INFO(g_logger) << "handleClient" << *client;
    }


    void TcpServer::startAccept(Socket::ptr sock)
    {
        while(!m_isStop)
        {
            Socket::ptr client = sock->accept();
            if(client)
            {
                client->setRecvTimeout(m_recvTimeout);
                m_worker->schedule(std::bind(&TcpServer::handleClient, shared_from_this(), client));
            }
            else
            {
                SNAIL_LOG_ERROR(g_logger) << "accept fail errno=" << errno
                    << " errstr=" << strerror(errno);
            }
        }
    }

}