#include "nethost.hpp"
#include "io.hpp"

namespace enet::structs
{
    NetHost::NetHost(const Address& address, size_t peerCount)
    : m_address(address), m_settings({peerCount, 0, 0, 0})
    {}

    NetHost::NetHost(std::string_view host, uint16_t port, size_t peerCount)
    : m_address(host, port), m_settings({peerCount, 0, 0, 0})
    {}

    NetHost::~NetHost() = default;

    void NetHost::SetAddress(const Address& address)
    {
        m_address = address;
    }

    void NetHost::SetAddress(std::string_view host, uint16_t port)
    {
        m_address.m_host = ::inet_addr(host.data());
        m_address.m_port = port;
    }

    void NetHost::SetPeerCount(size_t peerCount)
    {
        m_settings.m_peerCount = peerCount;
    }

    void NetHost::SetChannelLimit(size_t channelLimit)
    {
        m_settings.m_channelLimit = channelLimit;
    }

    void NetHost::SetIncomingBandwidth(uint32_t incomingBandwidth)
    {
        m_settings.m_incomingBandwidth = incomingBandwidth;
    }

    void NetHost::SetOutgoingBandwidth(uint32_t outgoingBandwidth)
    {
        m_settings.m_outgoingBandwidth = outgoingBandwidth;
    }

    void NetHost::StartHost()
    {}

    void NetHost::StopHost()
    {}

    void NetHost::send(const Msg& msg)
    {
        std::unique_lock lock(*m_SendMsgQueueMutex);
        m_SendMsgQueue->push(msg);
    }

    std::optional<Msg> NetHost::recv()
    {
        if (m_RecvMsgQueue->empty())
        {
            return std::nullopt;
        }
        else
        {
            std::unique_lock lock(*m_RecvMsgQueueMutex);
            Msg msg = m_RecvMsgQueue->front();
            m_RecvMsgQueue->pop();
            return msg;
        }
    }
        socket_t NetHost::handle() const
    {
        return m_socket;
    }

    void WorkerSend(std::stop_token stopToken, std::shared_ptr<std::queue<Msg>>& SendMsgQueue, std::shared_ptr<std::mutex>& SendMsgQueueMutex, Nethost& host)
    {
        while(!stopToken.stop_requested())
        {
            std::unique_lock lock(*SendMsgQueueMutex);
            auto& queue = *SendMsgQueue;
            if(queue.empty())
                continue;
            else
            {
                auto msg = queue.front();
                queue.pop();
                lock.unlock();
                // send msg
            }
        }
    }

    void WorkerRecv(std::stop_token stopToken, std::shared_ptr<std::queue<Msg>>& RecvMsgQueue, std::shared_ptr<std::mutex>& RecvMsgQueueMutex, Nethost& host)
    {
        while(!stopToken.stop_requested())
        {
            using enum enet::enums::Err;

            Msg msg;
            auto result = internal::RecvMsg(host.handle(), msg);
            switch (result)
            {
            case OK:
            {
                std::unique_lock lock(*RecvMsgQueueMutex);
                auto& queue = *RecvMsgQueue;
                queue.push(msg);
                break;
            }
            case DISCONNECTED:
            {
                // handle disconnect
                break;
            }
            default:
                break;
            }
        }
    }

    void NetHost::createHost()
    {
        if(m_activeHost)
            return;

        if(m_address.m_host != 0 && m_sr == nullptr)
        {
            m_SendMsgQueue = std::make_shared<std::queue<Msg>>();
            m_RecvMsgQueue = std::make_shared<std::queue<Msg>>();
            m_SendMsgQueueMutex = std::make_shared<std::mutex>();
            m_RecvMsgQueueMutex = std::make_shared<std::mutex>();

            auto addr = internal::GetAddressInfo(m_address.m_host, m_address.m_port, AF_INET, SOCK_STREAM, IPPROTO_TCP, 0);
            m_socket = internal::CreateSocket(m_addrinfo);
            if(m_socket == INVALID_SOCKET)
                elog::Error<"NET">("Failed to create socket");

            bool flag =  true;
            if(int result = ::setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(bool)); < 0)
                elog::Error<"NET">("Failed to set TCP_NODELAY");

            if(internal::ConnectToServer(m_socket, addr) != Sock_ERR)
                elog::Error<"NET">("Failed to connect to server");

            internal::FreeAddressInfo(addr);

            m_sr = std::make_shared<std::pair<std::jthread, std::jthread>>(
                std::jthread([this](std::stop_token stopToken)
                {
                    WorkerSend(stopToken, m_SendMsgQueue, m_SendMsgQueueMutex, this);
                }),
                std::jthread([this](std::stop_token stopToken)
                {
                    WorkerRecv(stopToken, m_RecvMsgQueue, m_RecvMsgQueueMutex, this);
                })
            );

            m_activeHost = true;
        }
        else if (m_address.m_host == 0 && m_listener == nullptr)
        {

        }
    }

    void NetHost::destroyHost()
    {

    }
}