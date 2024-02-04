#include "msgheader.hpp"

#include <unordered_map>

#include "msgorder.hpp"

namespace enet
{
    namespace structs
    {
        void MsgHeader::setMsgType(std::string_view type)
        {
            m_msgType = std::hash<std::string_view>{}(type);
        }
    }

    static std::unordered_map<size_t, structs::MsgOrder> g_msgOrderMap;
    
    namespace internal
    {
        std::unordered_map<size_t, structs::MsgOrder>& GetMsgOrderMap()
        {
            return g_msgOrderMap;
        }
    }
    
    bool AddNewMsgType(std::string_view type, structs::MsgOrder order)
    {
        auto [it, success] = g_msgOrderMap.try_emplace(std::hash<std::string_view>{}(type), order);
        return success;
    }

    std::optional<const std::reference_wrapper<structs::MsgOrder>> GetMsgOrder(std::string_view type)
    {
        auto it = g_msgOrderMap.find(std::hash<std::string_view>{}(type));
        if (it == g_msgOrderMap.end())
        {
            return std::nullopt;
        }
        return it->second;
    }
}