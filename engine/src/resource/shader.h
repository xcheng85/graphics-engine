#pragma once
#include "resource.h"

namespace ge::resource
{
    struct ShaderPayload
    {
    };

    class ShaderStage : public IResource
    {
    public:
        GE_INLINE ShaderStage() = default;
        GE_INLINE ~ShaderStage()
        {
        }

    private:
        ShaderPayload _payload;
    };
}