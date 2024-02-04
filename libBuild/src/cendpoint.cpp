#include "cendpoint.hpp"

namespace enet
{
    Client::Client(enums::SocketType type, std::string_view addr, std::string_view port, bool blocking)
        : m_socket(type, addr, port, blocking)
    {}

    Client::Client(enums::SocketType type, std::string_view addr, std::string_view port, enums::SocketDomainType domain, enums::SocketProtocol protocol, bool blocking)
        : m_socket(type, addr, port, domain, protocol, blocking)
    {}

    Client::Client(enums::SocketType type, std::string_view addr, std::string_view port, const structs::SocketSettings& settings)
        : m_socket(type, addr, port, settings)
    {}

    Client::Client(const structs::Socket& socket)
        : m_socket(socket)
    {}

    std::string_view Client::getAddr() const
    {
        return m_socket.getAddr();
    }

    std::string_view Client::getPort() const
    {
        return m_socket.getPort();
    }

    bool Client::connect() const
    {
        return m_socket.connect();
    }

    bool Client::send(structs::Msg& msg) const
    {
        return m_socket.send(msg);
    }

    std::optional<structs::Msg> Client::recv()
    {
        return m_socket.recv();
    }

    void Client::close()
    {
        m_socket.close();
    }
} // namespace enet::structs