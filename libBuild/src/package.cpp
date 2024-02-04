#include "package.hpp"

namespace enet
{
    namespace internal
    {
        structs::PackageList SplitData(const uint8_t* data, size_t len)
        {
            structs::PackageList packages;
            size_t numPackages = len / ::MAX_BUF;
            size_t lastPackageSize = len % ::MAX_BUF;

            for (size_t i = 0; i < numPackages; ++i)
            {
                structs::Package p;
                std::copy(data + i * ::MAX_BUF, data + (i + 1) * ::MAX_BUF, p.m_data.begin());
                p.m_size = ::MAX_BUF;
                packages.push_back(p);
            }

            if (lastPackageSize > 0)
            {
                structs::Package p;
                std::copy(data + numPackages * ::MAX_BUF, data + numPackages * ::MAX_BUF + lastPackageSize, p.m_data.begin());
                p.m_size = lastPackageSize;
                packages.push_back(p);
            }

            return packages;
        }
    }
}