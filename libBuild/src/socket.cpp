#include "socket.hpp"
#include "elog.hpp"
#include "package.hpp"
#include "msgorder.hpp"

namespace enet
{
    namespace structs
    {
        Socket::Socket(enums::SocketType type, std::string_view addr, std::string_view port)
            : m_sockfd(::INVAL_SOCK), m_addr(addr), m_port(port), m_type(type)
        {
            m_settings.domain = enums::SocketDomainType::IPv4;
            m_settings.protocol = enums::SocketProtocol::TCP;
            m_settings.blocking = true;

            generateAddrInfo();
            createSocket();
        }

        Socket::Socket(enums::SocketType type, std::string_view addr, std::string_view port, enums::SocketDomainType domain, enums::SocketProtocol protocol, bool blocking)
            : m_settings(), m_sockfd(0), m_addr(addr), m_port(port), m_type(type)
        {
            m_settings.domain = domain;
            m_settings.protocol = protocol;
            m_settings.blocking = blocking;

            generateAddrInfo();
            createSocket();
        }

        Socket::Socket(enums::SocketType type, std::string_view addr, std::string_view port, const structs::SocketSettings& settings)
            : m_settings(settings), m_sockfd(0), m_addr(addr), m_port(port), m_type(type)
        {
            generateAddrInfo();
            createSocket();
        }

        void Socket::setSettings(const structs::SocketSettings& settings)
        {
            m_settings = settings;
        }

        void Socket::setAddr(std::string_view addr)
        {
            m_addr = addr;
        }

        void Socket::setPort(std::string_view port)
        {
            m_port = port;
        }

        void Socket::setType(enums::SocketType type)
        {
            m_type = type;
        }

        void Socket::setDomain(enums::SocketDomainType domain)
        {
            m_settings.domain = domain;
        }

        void Socket::setProtocol(enums::SocketProtocol protocol)
        {
            m_settings.protocol = protocol;
        }

        void Socket::setBlocking(bool blocking)
        {
            m_settings.blocking = blocking;
        }

        std::string_view Socket::getAddr() const
        {
            return m_addr;
        }

        std::string_view Socket::getPort() const
        {
            return m_port;
        }

        enums::SocketType Socket::getType() const
        {
            return m_type;
        }

        enums::SocketDomainType Socket::getDomain() const
        {
            return m_settings.domain;
        }

        enums::SocketProtocol Socket::getProtocol() const
        {
            return m_settings.protocol;
        }

        bool Socket::getBlocking() const
        {
            return m_settings.blocking;
        }

        socket_t Socket::getFd() const
        {
            return m_sockfd;
        }

        bool Socket::bind()
        {
            if(m_type == enums::SocketType::NONE
            || m_type == enums::SocketType::Client
            || m_port.empty())
            {
                elog::Error<"NET">("Invalid socket type, address, or port");
                return false;
            }

            if(internal::Bind(m_sockfd, m_addrinfo) == -1)
            {
                elog::Error<"NET">("Bind failed");
                m_sockfd = ::SOCK_ERR;
                return false;
            }

            internal::FreeAddressInfo(m_addrinfo);
            return m_sockfd != ::SOCK_ERR;
        }

        bool Socket::listen()
        {
            if(m_type == enums::SocketType::NONE
            || m_type == enums::SocketType::Client
            || m_port.empty())
            {
                elog::Error<"NET">("Invalid socket type, address, or port");
                return false;
            }

            if(m_sockfd == ::SOCK_ERR)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(internal::Listen(m_sockfd) == -1)
            {
                elog::Error<"NET">("Listen failed");
                m_sockfd = ::SOCK_ERR;
                return false;
            }

            return true;
        }

        std::optional<Socket> Socket::accept()
        {
            if(m_type == enums::SocketType::NONE
            || m_type == enums::SocketType::Client
            || m_port.empty())
            {
                elog::Error<"NET">("Invalid socket type, address, or port");
                return std::nullopt;
            }

            if(m_sockfd == ::SOCK_ERR)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return std::nullopt;
            }

            char* addrStr = nullptr;
            char* portStr = nullptr;
            socket_t new_fd = internal::Accept(m_sockfd, &addrStr, &portStr);
            if (new_fd == ::SOCK_ERR)
            {
                elog::Error<"NET">("Accept failed");
                return std::nullopt;
            }

            Socket newSocket;
            newSocket.setFD(new_fd);
            newSocket.setAddr(addrStr);
            newSocket.setPort(portStr);
            newSocket.setSettings(m_settings);

            return newSocket;
        }

        bool Socket::connect()
        {
            if(m_type == enums::SocketType::NONE
            || m_type == enums::SocketType::Server
            || m_addr.empty()
            || m_port.empty())
            {
                elog::Error<"NET">("Invalid socket type, address, or port");
                return false;
            }

            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(internal::ConnectToServer(m_sockfd, m_addrinfo) == -1)
            {
                elog::Error<"NET">("Connect failed");
                return false;
            }

            return true;
        }

        bool Socket::send(Msg& msg, int flags)
        {
            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(m_type == enums::SocketType::NONE)
            {
                elog::Error<"NET">("Invalid socket type");
                return false;
            }

            msg.pack();

            if(!sendHeader(msg))
                return false;
            if(!sendOrder(msg))
                return false;
            if(!sendPackages(msg))
                return false;

            return true;
        }

        std::optional<Msg> Socket::recv(int flags)
        {
            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return std::nullopt;
            }

            if(m_type == enums::SocketType::NONE)
            {
                elog::Error<"NET">("Invalid socket type");
                return std::nullopt;
            }

            Msg msg;
            if(!recvHeader(msg))
                return std::nullopt;
            if(!recvOrder(msg))
                return std::nullopt;
            if(!recvPackages(msg))
                return std::nullopt;

            return msg;
        }

        bool Socket::sendTo(const Msg& msg, int flags)
        {
            return false;
        }

        std::optional<Msg> Socket::recvFrom(int flags)
        {
            return std::nullopt;
        }

        void Socket::close()
        {
            if (m_sockfd != ::INVAL_SOCK)
            {
                internal::CloseSocket(m_sockfd);
                m_sockfd = ::INVAL_SOCK;
            }
        }

        void Socket::createSocket()
        {
            if (m_sockfd != ::INVAL_SOCK)
            {
                close();
            }

            m_sockfd = internal::CreateSocket(m_addrinfo);
        }

        void Socket::generateAddrInfo()
        {
            int family = AF_INET;
            int streamType = SOCK_STREAM;
            int protocol = IPPROTO_TCP;
            internal::ToApiType(m_settings, family, streamType, protocol);

            int flag = 0;
            char* addr = m_addr.data();
            if(m_type == enums::SocketType::Server)
            {
                flag = AI_PASSIVE;
                addr = nullptr;
            }

            m_addrinfo = internal::GetAddressInfo(addr, m_port.c_str(), family, streamType, protocol, flag);
        }

        void Socket::setFD(socket_t sockfd)
        {
            m_sockfd = sockfd;
        }

        bool Socket::sendHeader(const Msg& msg)
        {
            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(m_type == enums::SocketType::NONE)
            {
                elog::Error<"NET">("Invalid socket type");
                return false;
            }

            const auto& header = msg.header();
            int64_t msgSize = sizeof(MsgHeader);

            if(int64_t bytesSent = internal::Send(m_sockfd, (const char*)&header, static_cast<int>(msgSize), 0); bytesSent == -1)
            {
                elog::Error<"NET">("Send failed: Header");
                return false;
            }

            return true;
        }

        bool Socket::sendOrder(const Msg& msg)
        {
            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(m_type == enums::SocketType::NONE)
            {
                elog::Error<"NET">("Invalid socket type");
                return false;
            }

            if(msg.order().count() == 0)
                return true;

            const auto& order = msg.order();
            int64_t msgSize = order.count();

            if(int64_t bytesSent = internal::Send(m_sockfd, (const char*)order.data(), static_cast<int>(msgSize), 0); bytesSent == -1)
            {
                elog::Error<"NET">("Send failed: Order");
                return false;
            }

            return true;
        }

        bool Socket::sendPackages(const Msg& msg)
        {
            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(m_type == enums::SocketType::NONE)
            {
                elog::Error<"NET">("Invalid socket type");
                return false;
            }

            auto packages = msg.getPackageList();
            int64_t bytesSent = 0;
            for(const auto& package : packages)
            {
                int64_t msgSize = package.m_size;
                int64_t current = 0;
                const auto* data = (const char*)package.m_data.data();
                if(current = internal::Send(m_sockfd, data, static_cast<int>(msgSize), 0); current == -1)
                    break;

                bytesSent += current;
            }

            if(bytesSent != msg.size())
            {
                elog::Error<"NET">("Send failed: Packages");
                return false;
            }

            return true;
        }

        bool Socket::recvHeader(Msg& msg)
        {
            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(m_type == enums::SocketType::NONE)
            {
                elog::Error<"NET">("Invalid socket type");
                return false;
            }

            MsgHeader header;
            int64_t msgSize = sizeof(MsgHeader);
            if(int64_t bytesRecv = internal::Recv(m_sockfd, (char*)&header, static_cast<int>(msgSize), 0); bytesRecv == -1 || bytesRecv != msgSize)
            {
                elog::Error<"NET">("Recv failed: Header");
                return false;
            }

            msg.header() = header;
            return true;
        }

        bool Socket::recvOrder(Msg& msg)
        {
            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(m_type == enums::SocketType::NONE)
            {
                elog::Error<"NET">("Invalid socket type");
                return false;
            }

            if(msg.header().m_msgOrderCount == 0)
                return true;

            MsgOrder order;
            order.getOrder().resize(msg.header().m_msgOrderCount);
            int64_t msgSize = msg.header().m_msgOrderCount;

            if(int64_t bytesRecv = internal::Recv(m_sockfd, (char*)order.getOrder().data(), static_cast<int>(msgSize), 0); bytesRecv == -1 || bytesRecv != msgSize)
            {
                elog::Error<"NET">("Recv failed: Order");
                return false;
            }

            msg.order() = order;
            return true;
        }

        bool Socket::recvPackages(Msg& msg)
        {
            if(m_sockfd == ::INVAL_SOCK)
            {
                elog::Error<"NET">("Invalid socket file descriptor");
                return false;
            }

            if(m_type == enums::SocketType::NONE)
            {
                elog::Error<"NET">("Invalid socket type");
                return false;
            }

            PackageList packages;
            int64_t bytesRecv = 0;
            int64_t msgSize = msg.header().m_msgSize;
            while(bytesRecv < msgSize)
            {
                Package package;
                int64_t current = 0;
                if(current = internal::Recv(m_sockfd, (char*)package.m_data.data(), static_cast<int>(package.m_size), 0); current == -1)
                    break;

                bytesRecv += current;
                package.m_size = current;
                packages.push_back(package);
            }

            if(bytesRecv != msgSize)
            {
                elog::Error<"NET">("Recv failed: Packages");
                return false;
            }

            msg.addPackageList(packages);
            return true;
        }
    } // namespace structs
} // namespace enet