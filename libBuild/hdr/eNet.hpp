#pragma once

#include "cendpoint.hpp"
#include "sendpoint.hpp"
#include "msg.hpp"
#include "err.hpp"

namespace enet
{
    enums::Err Init();
    void Shutdown();
}
