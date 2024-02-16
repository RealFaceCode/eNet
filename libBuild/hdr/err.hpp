#pragma once

namespace enet
{
    namespace enums
    {
        enum class Err
        {
        FAILED = 0,
        OK = 1,
        SEND_HEADER_FAILED = 2,
        RECV_HEADER_FAILED = 3,
        SEND_ORDER_FAILED = 4,
        RECV_ORDER_FAILED = 5,
        SEND_PACKAGES_FAILED = 6,
        RECV_PACKAGES_FAILED = 7,
        INVALID_CHECKSUM = 8,
        DISCONNECTED = 9,
        };

        bool operator! (Err err);
        bool operator== (Err err, bool b);
        bool operator!= (Err err, bool b);
        bool operator== (bool b, Err err);
        bool operator!= (bool b, Err err);

    }

    bool IsOK(enums::Err err);
} // namespace enet::enums