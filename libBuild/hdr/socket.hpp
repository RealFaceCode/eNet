#pragma once

#include <string>
#include <string_view>

#include "settings.hpp"
#include "netwrapper.hpp"
#include "msg.hpp"
#include "err.hpp"

namespace enet::structs
{
    struct Socket
    {
    public:
        Socket();
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

        enums::Err bind();
        enums::Err listen();
        std::optional<Socket> accept();
        enums::Err connect() const;
        enums::Err send(Msg& msg) const;
        std::optional<Msg> recv();
        enums::Err sendTo(const Msg& msg) const;
        std::optional<Msg> recvFrom() const;
        void close();

    private:
        void createSocket();
        void generateAddrInfo();
        void setFD(socket_t sockfd);

        enums::Err sendHeader(const Msg& msg) const;
        enums::Err sendOrder(const Msg& msg) const;
        enums::Err sendPackages(const Msg& msg) const;

        enums::Err recvHeader(Msg& msg) const;
        enums::Err recvOrder(Msg& msg) const;
        enums::Err recvPackages(Msg& msg) const;

        structs::SocketSettings m_settings;
        socket_t m_sockfd;
        addrinfo* m_addrinfo = nullptr;
        std::string m_addr;
        std::string m_port;
        enums::SocketType m_type;
    };
}
