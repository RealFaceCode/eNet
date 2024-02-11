#include "enet.hpp"

#include <elog.hpp>
#include "netwrapper.hpp"

namespace enet
{
    enums::Err Init()
    {
        elog::Init();
        return enet::internal::InitNetworking();
    }

    void Shutdown()
    {
        enet::internal::CleanupNetworking();
    }
}

