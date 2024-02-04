#include "settings.hpp"
#include "elog.hpp"
#include "netwrapper.hpp"

namespace enet
{
    namespace internal
    {
        void ToApiType(const structs::SocketSettings& settings, int& domain, int& type, int& protocol)
        {
            switch(settings.domain)
            {
                case enums::SocketDomainType::IPv4:
                    domain = AF_INET;
                    break;
                case enums::SocketDomainType::IPv6:
                    domain = AF_INET6;
                    break;
                default:
                    elog::Error<"NET">("Invalid domain type");
                    return;
            }

            switch(settings.type)
            {
                case enums::SocketStreamType::STREAM:
                    type = SOCK_STREAM;
                    break;
                case enums::SocketStreamType::DGRAM:
                    type = SOCK_DGRAM;
                    break;
                default:
                    elog::Error<"NET">("Invalid stream type");
                    return;
            }

            switch(settings.protocol)
            {
                case enums::SocketProtocol::TCP:
                    protocol = IPPROTO_TCP;
                    break;
                case enums::SocketProtocol::UDP:
                    protocol = IPPROTO_UDP;
                    break;
                default:
                    elog::Error<"NET">("Invalid protocol type");
                    return;
            }
        }
    }
}