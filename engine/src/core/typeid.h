#pragma once

namespace ge
{
    // template meta programming example
    GE_INLINE const uint32_t getUniqueTypeId()
    {
        static uint32_t type = 1u;
        return type++;
    }

    // each T will have a singleton counter
    template <typename T>
    GE_INLINE const uint32_t typeId()
    {
        static const uint32_t type = getUniqueTypeId();
        return type;
    }
}