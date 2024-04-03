#pragma once
#include "pch.h"

namespace ge::resource
{
    using resourceTypeId = uint32_t;
    using resourceId = uuid64;

    class IResource
    {
    public:
        GE_INLINE virtual ~IResource() = default;

    private:
        resourceId id{uuid()};
        std::string name;
    };
}
