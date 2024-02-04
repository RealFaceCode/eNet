#pragma once

#include "socket.hpp"

namespace enet
{
    struct Client
    {
    public:
        Client() = default;
        Client(enums::SocketType type, std::string_view addr, std::string_view port, bool blocking = true);
        Client(enums::SocketType type, std::string_view addr, std::string_view port, enums::SocketDomainType domain, enums::SocketProtocol protocol, bool blocking);
        Client(enums::SocketType type, std::string_view addr, std::string_view port, const structs::SocketSettings& settings);
        Client(const structs::Socket& socket);
        ~Client() = default;

        std::string_view getAddr() const;
        std::string_view getPort() const;

        bool connect() const;
        bool send(structs::Msg& msg) const;
        std::optional<structs::Msg> recv();
        void close();
    private:
        structs::Socket m_socket;
    };
} // namespace enet::structs