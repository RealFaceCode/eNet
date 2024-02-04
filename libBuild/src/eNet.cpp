#include "eNet.hpp"

#include <elog.hpp>
#include "netwrapper.hpp"

namespace enet
{
    void Init()
    {
        elog::Init();
        enet::internal::InitNetworking();
    }

    void Shutdown()
    {
        enet::internal::CleanupNetworking();
    }
}

