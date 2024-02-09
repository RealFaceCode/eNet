#pragma once

#include "socket.hpp"

namespace enet
{
    struct Client;

    struct Server
    {
    public:
        Server() = default;
        Server(std::string_view addr, std::string_view port, bool blocking = true);
        Server(std::string_view addr, std::string_view port, enums::SocketDomainType domain, enums::SocketProtocol protocol, bool blocking);
        Server(std::string_view addr, std::string_view port, const structs::SocketSettings& settings);
        ~Server() = default;

        std::string_view getAddr() const;
        std::string_view getPort() const;
        std::vector<Client>& getConnections();
        const std::vector<Client>& getConnections() const;

        void checkForConnections();
        void close();

        std::vector<Client>::iterator begin();
        std::vector<Client>::iterator end();
    private:
        structs::Socket m_socket;

        std::vector<Client> connections;
    };
} // namespace enet::structs