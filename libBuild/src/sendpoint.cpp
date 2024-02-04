#include "sendpoint.hpp"
#include "cendpoint.hpp"

namespace enet
{
    Server::Server(enums::SocketType type, std::string_view addr, std::string_view port, bool blocking)
        : m_socket(type, addr, port, blocking)
    {}

    Server::Server(enums::SocketType type, std::string_view addr, std::string_view port, enums::SocketDomainType domain, enums::SocketProtocol protocol, bool blocking)
        : m_socket(type, addr, port, domain, protocol, blocking)
    {}

    Server::Server(enums::SocketType type, std::string_view addr, std::string_view port, const structs::SocketSettings& settings)
        : m_socket(type, addr, port, settings)
    {}

    std::string_view Server::getAddr() const
    {
        return m_socket.getAddr();
    }

    std::string_view Server::getPort() const
    {
        return m_socket.getPort();
    }

    std::vector<Client>& Server::getConnections()
    {
        return connections;
    }

    const std::vector<Client>& Server::getConnections() const
    {
        return connections;
    }

    void Server::checkForConnections()
    {
        auto client = m_socket.accept();
        if (client.has_value())
            connections.emplace_back(client.value());
    }

    bool Server::send(structs::Msg& msg) const
    {
        return m_socket.send(msg);
    }

    std::optional<structs::Msg> Server::recv()
    {
        return m_socket.recv();
    }

    void Server::close()
    {
        m_socket.close();
    }

    std::vector<Client>::iterator Server::begin()
    {
        return connections.begin();
    }

    std::vector<Client>::iterator Server::end()
    {
        return connections.end();
    }
} // namespace enet::structs
