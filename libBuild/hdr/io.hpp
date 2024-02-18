#pragma once
#include "netwrapper.hpp"

namespace enet
{
    namespace enums
    {
        enum class Err;
    }

    namespace structs
    {
        struct Msg;
    }

    namespace internal
    {
        void SwapHeaderEndianHtoN(structs::Msg& msg);
        void SwapHeaderEndianNtoH(structs::Msg& msg);
        enums::Err SendHeader(socket_t fd, structs::Msg& msg);
        enums::Err RecvHeader(socket_t fd, structs::Msg& msg);
        enums::Err SendOrder(socket_t fd, structs::Msg& msg);
        enums::Err RecvOrder(socket_t fd, structs::Msg& msg);
        enums::Err SendPackages(socket_t fd, structs::Msg& msg);
        enums::Err RecvPackages(socket_t fd, structs::Msg& msg);
        enums::Err SendMsg(socket_t fd, structs::Msg& msg);
        enums::Err RecvMsg(socket_t fd, structs::Msg& msg);
    }
}