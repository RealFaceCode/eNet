#pragma once

#include <cstdint>
#include <vector>
#include <string_view>
#include <optional>

namespace enet
{
    namespace enums
    {
        enum class MsgOrderType : uint8_t
        {
            String = 0,
            UInt8 = 1,
            Int8 = 2,
            UInt16 = 3,
            Int16 = 4,
            UInt32 = 5,
            Int32 = 6,
            UInt64 = 7,
            Int64 = 8,
            Float = 9,
            Double = 10
        };
    }
    
    namespace structs
    {
        struct MsgOrder
        {
        public:
            MsgOrder() = default;
            explicit MsgOrder(const std::vector<enums::MsgOrderType>& order);
            ~MsgOrder() = default;

            void setOrder(const std::vector<enums::MsgOrderType>& order);
            std::vector<enums::MsgOrderType>& getOrder();
            const std::vector<enums::MsgOrderType>& getOrder() const;

            enums::MsgOrderType* data();
            const enums::MsgOrderType* data() const;

            size_t count() const;

            std::vector<enums::MsgOrderType>::iterator begin();
            std::vector<enums::MsgOrderType>::iterator end();

            std::vector<enums::MsgOrderType>::const_iterator begin() const;
            std::vector<enums::MsgOrderType>::const_iterator end() const;

            std::vector<enums::MsgOrderType>::const_iterator cbegin() const;
            std::vector<enums::MsgOrderType>::const_iterator cend() const;

            std::vector<enums::MsgOrderType>::reverse_iterator rbegin();
            std::vector<enums::MsgOrderType>::reverse_iterator rend();

            std::vector<enums::MsgOrderType>::const_reverse_iterator crbegin() const;
            std::vector<enums::MsgOrderType>::const_reverse_iterator crend() const;
        private:
            std::vector<enums::MsgOrderType> m_order;
        };
    }

    bool AddMsgOrder(const std::string& order, const structs::MsgOrder& msgOrder);
    bool AddMsgOrder(const std::string& order, const std::vector<enums::MsgOrderType>& msgOrder);
    bool AddMsgOrder(const std::string& order, const enums::MsgOrderType* msgOrder, size_t count);

    bool RemoveMsgOrder(const std::string& order);
    bool RemoveAllMsgOrders();

    const std::optional<structs::MsgOrder> GetMsgOrder(const std::string& order);
}