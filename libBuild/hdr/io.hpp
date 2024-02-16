#pragma once

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
        enums::Err SendHeader(int fd, structs::Msg& msg);
        enums::Err RecvHeader(int fd, structs::Msg& msg);
        enums::Err SendOrder(int fd, structs::Msg& msg);
        enums::Err RecvOrder(int fd, structs::Msg& msg);
        enums::Err SendPackages(int fd, structs::Msg& msg);
        enums::Err RecvPackages(int fd, structs::Msg& msg);
    }
}