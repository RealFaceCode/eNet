#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include "netwrapper.hpp"

namespace enet
{
    namespace structs
    {
        struct Package
        {
        public:
            std::array<uint8_t, ::MAX_BUF> m_data;
            size_t m_size;
        };

        using PackageList = std::vector<Package>;
    }

    namespace internal
    {
        structs::PackageList SplitData(const uint8_t* data, size_t len);
    }
}