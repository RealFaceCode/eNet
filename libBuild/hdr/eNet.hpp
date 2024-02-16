#pragma once

#include "msg.hpp"
#include "err.hpp"

namespace enet
{
    enums::Err Init();
    void Shutdown();
}
