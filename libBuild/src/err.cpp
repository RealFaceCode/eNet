#include "err.hpp"

namespace enet::enums
{
    bool operator! (Err err)
    {
        return err == Err::OK;
    }
} // namespace enet::enums