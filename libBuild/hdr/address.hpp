#pragma once

#include <cstdint>

namespace enet
{
    struct Address
    {
    public:
        Address();
        Address(uint32_t host, uint16_t port);
        Adress(std::string_view host, uint16_t port);
        ~Address();

        void SetHost(std::string_view host);
        void SetHost(uint32_t host);
        void SetPort(uint16_t port);

        uint32_t m_host = 0;
        uint16_t m_port = 0;
    };
}