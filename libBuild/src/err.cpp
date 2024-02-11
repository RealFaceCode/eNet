#include "err.hpp"

namespace enet
{
    namespace enums
    {
        bool operator! (Err err)
        {
            return err == Err::OK;
        }
    }

    bool IsOK(enums::Err err)
    {
        return err == enums::Err::OK || err == enums::Err::WOULD_NOT_BLOCK;
    }
} // namespace enet::enums