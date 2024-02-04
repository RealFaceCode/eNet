#pragma once

namespace enet
{
    namespace enums
    {
        enum class SocketDomainType
        {
            NONE = 0,
            IPv4 = 1,
            IPv6 = 2
        };

        enum class SocketStreamType
        {
            NONE = 0,
            STREAM = 1,
            DGRAM = 2
        };

        enum class SocketProtocol
        {
            NONE = 0,
            TCP = 1,
            UDP = 2
        };

        enum class SocketType
        {
            NONE = 0,
            Client = 1,
            Server = 2
        };
    }

    namespace structs
    {
        struct SocketSettings
        {
        public:
            enums::SocketDomainType domain = enums::SocketDomainType::IPv4;
            enums::SocketStreamType type = enums::SocketStreamType::STREAM;
            enums::SocketProtocol protocol = enums::SocketProtocol::TCP;
            bool blocking;
        };
    }

    namespace internal
    {
        void ToApiType(const structs::SocketSettings& settings, int& domain, int& type, int& protocol);
    }
}
