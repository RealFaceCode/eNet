#include "err.hpp"

namespace enet
{
    namespace enums
    {
        bool operator! (Err err)
        {
            return err != Err::OK;
        }

        bool operator== (Err err, bool b)
        {
            return static_cast<bool>(err) == b;
        }

        bool operator!= (Err err, bool b)
        {
            return static_cast<bool>(err) != b;
        }

        bool operator== (bool b, Err err)
        {
            return b == static_cast<bool>(err);
        }

        bool operator!= (bool b, Err err)
        {
            return b != static_cast<bool>(err);
        }
    }

    bool IsOK(enums::Err err)
    {
        bool r = err == enums::Err::OK || err == enums::Err::WOULD_NOT_BLOCK;
        return r;
    }
} // namespace enet::enums