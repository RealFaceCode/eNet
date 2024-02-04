#pragma once

#include <vector>
#include <cstdint>
#include <string_view>
#include <string>

#include "msgheader.hpp"
#include "settings.hpp"
#include "package.hpp"

namespace enet
{
    namespace structs
    {
        struct Msg
        {
        public:
            Msg() = default;
            explicit Msg(std::string_view msgType);
            ~Msg();

            void append(const uint8_t* data, size_t len);
            void append(const std::vector<uint8_t>& data);
            void append(std::string_view data);
            void append(const char* data, size_t len);
            void append(const char* data);
            void append(uint8_t data);
            void append(int8_t data);
            void append(uint16_t data);
            void append(int16_t data);
            void append(uint32_t data);
            void append(int32_t data);
            void append(uint64_t data);
            void append(int64_t data);
            void append(float data);
            void append(double data);

            std::vector<uint8_t> getVector(size_t len);
            std::string getString();
            uint8_t getUI8();
            int8_t getI8();
            uint16_t getUI16();
            int16_t getI16();
            uint32_t getUI32();
            int32_t getI32();
            uint64_t getUI64();
            int64_t getI64();
            float getFloat();
            double getDouble();

            void clear();
            size_t size() const;
            const uint8_t* data() const;
            MsgHeader& header();
            const MsgHeader& header() const;
            MsgOrder& order();
            const MsgOrder& order() const;

            PackageList getPackageList() const;
            void addPackageList(const PackageList& list);
            void pack();

        private:
            std::vector<uint8_t> m_data;
            MsgHeader m_header;
            enums::SocketType m_type;
            MsgOrder m_order;
        };
    }
}