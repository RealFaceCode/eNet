#include "io.hpp"
#include "package.hpp"
#include "netwrapper.hpp"
#include "msg.hpp"
#include "err.hpp"
#include "msgorder.hpp"
#include "msgheader.hpp"

namespace enet::internal
{
    void SwapHeaderEndianHtoN(structs::Msg& msg)
    {
        auto& header = msg.header();
        header.m_msgSize = ::htonll(header.m_msgSize);
        header.m_msgType = ::htonll(header.m_msgType);
        header.m_msgOrderCount = ::htonll(header.m_msgOrderCount);
        header.m_checksum = ::htonll(header.m_checksum);
    }

    void SwapHeaderEndianNtoH(structs::Msg& msg)
    {
        auto& header = msg.header();
        header.m_msgSize = ::ntohll(header.m_msgSize);
        header.m_msgType = ::ntohll(header.m_msgType);
        header.m_msgOrderCount = ::ntohll(header.m_msgOrderCount);
        header.m_checksum = ::ntohll(header.m_checksum);
    }

    enums::Err SendHeader(int fd, structs::Msg& msg)
    {
        using enum enet::enums::Err;
        SwapHeaderEndianHtoN(msg);

        const auto& header = msg.header();

        if(auto result = internal::Send(fd, (const char*)&header, sizeof(structs::MsgHeader), 0); result == 0)
            return DISCONNECTED;
        else if (result == -1 || result != sizeof(structs::MsgHeader))
            return SEND_HEADER_FAILED;

        return OK;
    }

    enums::Err RecvHeader(int fd, structs::Msg& msg)
    {
        using enum enet::enums::Err;
        auto& header = msg.header();

        if(auto result = internal::Recv(fd, (char*)&header, sizeof(structs::MsgHeader), 0); result == 0)
            return DISCONNECTED;
        else if (result == -1 || result != sizeof(structs::MsgHeader))
            return RECV_HEADER_FAILED;

        SwapHeaderEndianNtoH(msg);
        return OK;
    }

    enums::Err SendOrder(int fd, structs::Msg& msg)
    {
        using enum enet::enums::Err;

        if(const auto& header = msg.header(); header.m_msgOrderCount == 0)
            return OK;

        const auto& order = msg.order();
        auto orderSize = sizeof(enums::MsgOrderType) * order.count();

        if(auto result = internal::Send(fd, (const char*)order.data(), static_cast<int>(orderSize), 0); result == 0)
            return DISCONNECTED;
        else if (result == -1 || result != orderSize)
            return SEND_ORDER_FAILED;

        return OK;
    }

    enums::Err RecvOrder(int fd, structs::Msg& msg)
    {
        using enum enet::enums::Err;

        const auto& header = msg.header();
        if( header.m_msgOrderCount == 0)
            return OK;

        auto& order = msg.order();
        order.reserve(header.m_msgOrderCount);

        auto orderSize = sizeof(enums::MsgOrderType) * header.m_msgOrderCount;

        if(auto result = internal::Recv(fd, (char*)order.data(), static_cast<int>(orderSize), 0); result == 0)
            return DISCONNECTED;
        else if (result == -1 || result != orderSize)
            return RECV_ORDER_FAILED;

        msg.order().setOrder(order.get());

        return OK;
    }

    enums::Err SendPackages(int fd, structs::Msg& msg)
    {
        using enum enet::enums::Err;

        const auto& header = msg.header();
        if(header.m_msgSize == 0)
            return OK;
        
        auto packages = msg.getPackageList();
        
        size_t totalBytes = 0;

        for(const auto& package : packages)
        {
            auto result = internal::Send(fd, (const char*)package.m_data.data(), static_cast<int>(package.m_size), 0);
            if(result == 0 )
                return DISCONNECTED;
            else if (result == -1 || result != package.m_size)
                return SEND_PACKAGES_FAILED;
            
            totalBytes += result;
        }

        if(totalBytes != header.m_msgSize)
            return SEND_PACKAGES_FAILED;

        return OK;
    }

    enums::Err RecvPackages(int fd, structs::Msg& msg)
    {
        using enum enet::enums::Err;

        const auto& header = msg.header();
        if(header.m_msgSize == 0)
            return OK;

        structs::PackageList packages;

        size_t totalBytes = 0;
        size_t remainingBytes = header.m_msgSize;

        while(remainingBytes > 0)
        {
            structs::Package package;
            package.m_size = remainingBytes > ::MAX_BUF ? ::MAX_BUF : remainingBytes;

            auto result = internal::Recv(fd, (char*)package.m_data.data(), static_cast<int>(package.m_size), 0);
            if(result == 0)
                return DISCONNECTED;
            else if (result == -1 || result != package.m_size)
                return RECV_PACKAGES_FAILED;

            totalBytes += result;
            remainingBytes -= result;
            
            packages.push_back(package);
        }

        if(totalBytes != header.m_msgSize)
            return RECV_PACKAGES_FAILED;

        msg.addPackageList(packages);

        return OK;
    }
}
