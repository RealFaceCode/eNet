#pragma once

#include <cstdint>
#include <string_view>

namespace enet
{
    struct Address
    {
    public:
        Address();
        Address(uint32_t host, uint16_t port);
        Address(std::string_view host, uint16_t port);
        ~Address();

        void SetHost(std::string_view host);

        uint32_t m_host = 0;
        uint16_t m_port = 0;
    };
}