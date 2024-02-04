#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>

#include "msgorder.hpp"

namespace enet
{
    namespace structs
    {
        struct MsgOrder;

        struct MsgHeader
        {
        public:
            void setMsgType(std::string_view type);

            size_t m_msgSize = 0;
            size_t m_msgType = 0;
            size_t m_msgOrderCount = 0;
        };
    }

    namespace internal
    {
        std::unordered_map<size_t, structs::MsgOrder>& GetMsgOrderMap();
    }

    bool AddNewMsgType(std::string_view type, structs::MsgOrder order);
    std::optional<const std::reference_wrapper<structs::MsgOrder>> GetMsgOrder(std::string_view type); 
}