#include "msg.hpp"

#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

namespace enet::structs
{
    Msg::Msg(std::string_view type)
    {
        m_header.setMsgType(type);

        auto order = GetMsgOrder(type);
        if (order.has_value())
            m_order = order.value().get();
        
    }

    Msg::~Msg()
    {
        clear();
    }

    void Msg::append(const uint8_t* data, size_t len)
    {
        m_data.insert(m_data.end(), data, data + len);
    }

    void Msg::append(const std::vector<uint8_t>& data)
    {
        m_data.insert(m_data.end(), data.begin(), data.end());
    }

    void Msg::append(std::string_view data)
    {
        append(data.size());
        m_data.insert(m_data.end(), data.begin(), data.end());
    }

    void Msg::append(const char* data, size_t len)
    {
        append(len);
        m_data.insert(m_data.end(), data, data + len);
    }

    void Msg::append(const char* data)
    {
        size_t len = ::strlen(data);
        append(len);
        m_data.insert(m_data.end(), data, data + len);
    }

    void Msg::append(uint8_t data)
    {
        m_data.push_back(data);
    }

    void Msg::append(int8_t data)
    {
        m_data.push_back(data);
    }

    void Msg::append(uint16_t data)
    {
        data = ::htons(data);
        m_data.push_back((data >> 8) & 0xFF);
        m_data.push_back(data & 0xFF);
    }

    void Msg::append(int16_t data)
    {
        data = ::htons(data);
        m_data.push_back((data >> 8) & 0xFF);
        m_data.push_back(data & 0xFF);
    }

    void Msg::append(uint32_t data)
    {
        data = ::htonl(data);
        m_data.push_back((data >> 24) & 0xFF);
        m_data.push_back((data >> 16) & 0xFF);
        m_data.push_back((data >> 8) & 0xFF);
        m_data.push_back(data & 0xFF);
    }

    void Msg::append(int32_t data)
    {
        data = ::htonl(data);
        m_data.push_back((data >> 24) & 0xFF);
        m_data.push_back((data >> 16) & 0xFF);
        m_data.push_back((data >> 8) & 0xFF);
        m_data.push_back(data & 0xFF);
    }

    void Msg::append(uint64_t data)
    {
        data = ::htonll(data);
        m_data.push_back((data >> 56) & 0xFF);
        m_data.push_back((data >> 48) & 0xFF);
        m_data.push_back((data >> 40) & 0xFF);
        m_data.push_back((data >> 32) & 0xFF);
        m_data.push_back((data >> 24) & 0xFF);
        m_data.push_back((data >> 16) & 0xFF);
        m_data.push_back((data >> 8) & 0xFF);
        m_data.push_back(data & 0xFF);
    }

    void Msg::append(int64_t data)
    {
        data = ::htonll(data);
        m_data.push_back((data >> 56) & 0xFF);
        m_data.push_back((data >> 48) & 0xFF);
        m_data.push_back((data >> 40) & 0xFF);
        m_data.push_back((data >> 32) & 0xFF);
        m_data.push_back((data >> 24) & 0xFF);
        m_data.push_back((data >> 16) & 0xFF);
        m_data.push_back((data >> 8) & 0xFF);
        m_data.push_back(data & 0xFF);
    }

    void Msg::append(float data)
    {
        const auto* ptr = (const uint32_t*)&data;
        uint32_t val = ::htonl(*ptr);
        append(val);
    }

    void Msg::append(double data)
    {
        const auto* ptr = (const uint64_t*)&data;
        uint64_t val = ::htonll(*ptr);
        append(val);
    }

    std::vector<uint8_t> Msg::getVector(size_t len)
    {
        std::vector<uint8_t> ret;
        ret.insert(ret.end(), m_data.begin(), m_data.begin() + len);
        m_data.erase(m_data.begin(), m_data.begin() + len);
        return ret;
    }

    std::string Msg::getString()
    {
        std::string ret;
        size_t len = getUI64();

        if(len > m_data.size())
            len = m_data.size();

        ret.insert(ret.end(), m_data.begin(), m_data.begin() + len);
        m_data.erase(m_data.begin(), m_data.begin() + len);
        return ret;
    }

    uint8_t Msg::getUI8()
    {
        uint8_t ret = m_data[0];
        m_data.erase(m_data.begin());
        return ret;
    }

    int8_t Msg::getI8()
    {
        int8_t ret = m_data[0];
        m_data.erase(m_data.begin());
        return ret;
    }

    uint16_t Msg::getUI16()
    {
        auto ret = static_cast<uint16_t>((static_cast<uint64_t>(m_data[0]) << 8) | m_data[1]);
        m_data.erase(m_data.begin(), m_data.begin() + 2);
        return ::ntohs(ret);
    }

    int16_t Msg::getI16()
    {
        auto ret = static_cast<int16_t>((static_cast<uint64_t>(m_data[0]) << 8) | m_data[1]);
        m_data.erase(m_data.begin(), m_data.begin() + 2);
        return ::ntohs(ret);
    }

    uint32_t Msg::getUI32()
    {
        auto ret = static_cast<uint32_t>((static_cast<uint64_t>(m_data[0]) << 24)
                    | (static_cast<uint64_t>(m_data[1]) << 16)
                    | (static_cast<uint64_t>(m_data[2]) << 8)
                    | static_cast<uint64_t>(m_data[3]));
        m_data.erase(m_data.begin(), m_data.begin() + 4);
        return ::ntohl(ret);
    }

    int32_t Msg::getI32()
    {
        auto ret = static_cast<int32_t>((static_cast<uint64_t>(m_data[0]) << 24)
                    | (static_cast<uint64_t>(m_data[1]) << 16)
                    | (static_cast<uint64_t>(m_data[2]) << 8)
                    | static_cast<uint64_t>(m_data[3]));
        m_data.erase(m_data.begin(), m_data.begin() + 4);
        return ::ntohl(ret);
    }

    uint64_t Msg::getUI64()
    {
        uint64_t ret = ((uint64_t)m_data[0] << 56)
                    | ((uint64_t)m_data[1] << 48)
                    | ((uint64_t)m_data[2] << 40)
                    | ((uint64_t)m_data[3] << 32)
                    | ((uint64_t)m_data[4] << 24)
                    | ((uint64_t)m_data[5] << 16)
                    | ((uint64_t)m_data[6] << 8)
                    | (uint64_t)m_data[7];
        m_data.erase(m_data.begin(), m_data.begin() + 8);
        return ::ntohll(ret);
    }

    int64_t Msg::getI64()
    {
        int64_t ret = ((int64_t)m_data[0] << 56)
                    | ((int64_t)m_data[1] << 48)
                    | ((int64_t)m_data[2] << 40)
                    | ((int64_t)m_data[3] << 32)
                    | ((int64_t)m_data[4] << 24)
                    | ((int64_t)m_data[5] << 16)
                    | ((int64_t)m_data[6] << 8)
                    | (int64_t)m_data[7];
        m_data.erase(m_data.begin(), m_data.begin() + 8);
        return ::ntohll(ret);
    }

    float Msg::getFloat()
    {
        uint32_t ret = getUI32();
        return (float)ret;
    }

    double Msg::getDouble()
    {
        uint64_t ret = getUI64();
        return (double)ret;
    }

    void Msg::clear()
    {
        m_data.clear();
    }

    size_t Msg::size() const
    {
        return m_data.size();
    }

    const uint8_t* Msg::data() const
    {
        return m_data.data();
    }

    MsgHeader& Msg::header()
    {
        return m_header;
    }

    const MsgHeader& Msg::header() const
    {
        return m_header;
    }

    MsgOrder& Msg::order()
    {
        return m_order;
    }

    const MsgOrder& Msg::order() const
    {
        return m_order;
    }

    PackageList Msg::getPackageList() const
    {
        return internal::SplitData(m_data.data(), m_data.size());
    }

    void Msg::addPackageList(const PackageList& list)
    {
        for (const auto& p : list)
        {
            m_data.insert(m_data.end(), p.m_data.begin(), p.m_data.begin() + p.m_size);
        }
    }
} // namespace enet::structs
