#pragma once

namespace enet::enums
{
    enum class Err
    {
        FAILED = 0,
        OK = 1,
        WOULD_NOT_BLOCK = 2,
        INVAL_SOCK_FD = 3,
        INVAL_SOCK_TYPE = 4,
        SEND_HEADER_FAILED = 5,
        SEND_ORDER_FAILED = 6,
        SEND_PACKAGES_FAILED = 7,
        RECV_HEADER_FAILED = 8,
        RECV_ORDER_FAILED = 9,
        RECV_PACKAGES_FAILED = 10,
        BIND_FAILED = 11,
        LISTEN_FAILED = 12,
        ACCEPT_FAILED = 13,
        CONNECT_FAILED = 14,
        CLOSE_FAILED = 15,
    };

    bool operator! (Err err);

} // namespace enet::enums