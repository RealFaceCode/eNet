#include "sendpoint.hpp"
#include "cendpoint.hpp"

namespace enet
{
    Server::Server(std::string_view addr, std::string_view port, bool blocking)
        : m_socket(enums::SocketType::Server, addr, port, blocking)
    {
        m_socket.bind();
        m_socket.listen();
    }

    Server::Server(std::string_view addr, std::string_view port, enums::SocketDomainType domain, enums::SocketProtocol protocol, bool blocking)
        : m_socket(enums::SocketType::Server, addr, port, domain, protocol, blocking)
    {
        m_socket.bind();
        m_socket.listen();
    }

    Server::Server(std::string_view addr, std::string_view port, const structs::SocketSettings& settings)
        : m_socket(enums::SocketType::Server, addr, port, settings)
    {
        m_socket.bind();
        m_socket.listen();
    }

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

    void Server::close()
    {
        for(auto& client : connections)
            client.close();
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
