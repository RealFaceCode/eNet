#pragma once

namespace enet
{
    namespace enums
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
        DISCONNECTED = 16,
        INVALID_MSG_TYPE = 17,
        INVALID_MSG_ORDER = 18,
        INVALID_MSG_SIZE = 19,
        INVALID_MSG_CHECKSUM = 20,
        };

        bool operator! (Err err);
        bool operator== (Err err, bool b);
        bool operator!= (Err err, bool b);
        bool operator== (bool b, Err err);
        bool operator!= (bool b, Err err);

    }

    bool IsOK(enums::Err err);
} // namespace enet::enums