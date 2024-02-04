#pragma once

#include <string>
#include <string_view>

#include "settings.hpp"
#include "netwrapper.hpp"
#include "msg.hpp"

namespace enet
{
    namespace structs
    {
        struct Socket
        {
        public:
            Socket() = default;
            Socket(enums::SocketType type, std::string_view addr, std::string_view port, bool blocking = true);
            Socket(enums::SocketType type, std::string_view addr, std::string_view port, enums::SocketDomainType domain, enums::SocketProtocol protocol, bool blocking);
            Socket(enums::SocketType type, std::string_view addr, std::string_view port, const structs::SocketSettings& settings);
            ~Socket() = default;

            void setSettings(const structs::SocketSettings& settings);
            void setAddr(std::string_view addr);
            void setPort(std::string_view port);
            void setType(enums::SocketType type);
            void setDomain(enums::SocketDomainType domain);
            void setProtocol(enums::SocketProtocol protocol);
            void setBlocking(bool blocking);

            std::string_view getAddr() const;
            std::string_view getPort() const;
            enums::SocketType getType() const;
            enums::SocketDomainType getDomain() const;
            enums::SocketProtocol getProtocol() const;
            bool getBlocking() const;
            socket_t getFd() const;

            bool bind();
            bool listen();
            std::optional<Socket> accept();
            bool connect() const;
            bool send(Msg& msg, int flags = 0);
            std::optional<Msg> recv(int flags = 0);
            bool sendTo(const Msg& msg, int flags = 0);
            std::optional<Msg> recvFrom(int flags = 0);
            void close();

        private:
            void createSocket();
            void generateAddrInfo();
            void setFD(socket_t sockfd);

            bool sendHeader(const Msg& msg) const;
            bool sendOrder(const Msg& msg) const;
            bool sendPackages(const Msg& msg) const;

            bool recvHeader(Msg& msg) const;
            bool recvOrder(Msg& msg) const;
            bool recvPackages(Msg& msg) const;

            structs::SocketSettings m_settings;
            socket_t m_sockfd;
            addrinfo* m_addrinfo;
            std::string m_addr;
            std::string m_port;
            enums::SocketType m_type;
        };
    }
}