#include "address.hpp"
#include "netwrapper.hpp"

namespace enet
{
    Address::Address() = default;

    Address::Address(uint32_t host, uint16_t port)
    : m_host(host), m_port(port)
    {}

    Address::Address(std::string_view host, uint16_t port)
    : m_port(port)
    {
        SetHost(host);
    }

    Address::~Address() = default;

    void Address::SetHost(std::string_view host)
    {
        m_host = ::inet_addr(host.data());
    }
}