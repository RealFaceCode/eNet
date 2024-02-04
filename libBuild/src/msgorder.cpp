#include "msgorder.hpp"

#include <unordered_map>
#include <string>

namespace enet
{
    namespace structs
    {
        MsgOrder::MsgOrder(const std::vector<enums::MsgOrderType>& order)
        : m_order(order)
        {}

        void MsgOrder::setOrder(const std::vector<enums::MsgOrderType>& order)
        {
            m_order = order;
        }

        std::vector<enums::MsgOrderType>& MsgOrder::getOrder()
        {
            return m_order;
        }

        const std::vector<enums::MsgOrderType>& MsgOrder::getOrder() const
        {
            return m_order;
        }

        enums::MsgOrderType* MsgOrder::data()
        {
            return m_order.data();
        }

        const enums::MsgOrderType* MsgOrder::data() const
        {
            return m_order.data();
        }

        size_t MsgOrder::count() const
        {
            return m_order.size();
        }

        std::vector<enums::MsgOrderType>::iterator MsgOrder::begin()
        {
            return m_order.begin();
        }

        std::vector<enums::MsgOrderType>::iterator MsgOrder::end()
        {
            return m_order.end();
        }

        std::vector<enums::MsgOrderType>::const_iterator MsgOrder::begin() const
        {
            return m_order.begin();
        }

        std::vector<enums::MsgOrderType>::const_iterator MsgOrder::end() const
        {
            return m_order.end();
        }

        std::vector<enums::MsgOrderType>::const_iterator MsgOrder::cbegin() const
        {
            return m_order.cbegin();
        }

        std::vector<enums::MsgOrderType>::const_iterator MsgOrder::cend() const
        {
            return m_order.cend();
        }

        std::vector<enums::MsgOrderType>::reverse_iterator MsgOrder::rbegin()
        {
            return m_order.rbegin();
        }

        std::vector<enums::MsgOrderType>::reverse_iterator MsgOrder::rend()
        {
            return m_order.rend();
        }

        std::vector<enums::MsgOrderType>::const_reverse_iterator MsgOrder::crbegin() const
        {
            return m_order.crbegin();
        }

        std::vector<enums::MsgOrderType>::const_reverse_iterator MsgOrder::crend() const
        {
            return m_order.crend();
        }
    }

    static std::unordered_map<std::string, structs::MsgOrder> MsgOrders;

    bool AddMsgOrder(const std::string& order, const structs::MsgOrder& msgOrder)
    {
        
        auto [it, success] = MsgOrders.try_emplace(order, msgOrder);
        return success;
    }

    bool AddMsgOrder(const std::string& order, const std::vector<enums::MsgOrderType>& msgOrder)
    {
        auto [it, success] = MsgOrders.try_emplace(order, msgOrder);
        return success;
    }

    bool AddMsgOrder(const std::string& order, const enums::MsgOrderType* msgOrder, size_t count)
    {
        auto [it, success] = MsgOrders.try_emplace(order, std::vector<enums::MsgOrderType>(msgOrder, msgOrder + count));
        return success;
    }

    bool RemoveMsgOrder(const std::string& order)
    {
        return MsgOrders.erase(order) > 0;
    }

    bool RemoveAllMsgOrders()
    {
        MsgOrders.clear();
        return true;
    }

    const std::optional<structs::MsgOrder> GetMsgOrder(const std::string& order)
    {
        auto it = MsgOrders.find(order);
        if (it == MsgOrders.end())
        {
            return std::nullopt;
        }

        return it->second;
    }
}
