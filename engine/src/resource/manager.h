#pragma once
#include "pch.h"
#include "core/typeid.h"
#include "resource.h"
#include "shader.h"

extern std::pmr::monotonic_buffer_resource ge::appAllocator;

namespace ge::resource
{
    class ResourceManager
    {
        GE_INLINE ResourceManager() = default;

        GE_INLINE void clear()
        {
            // pmr allocated on stack
            _lookup.clear();
        }

        template <std::derived_from<IResource> T>
        GE_INLINE T *get(resourceId id)
        {
            const resourceTypeId resourceTypeId = typeId<T>();
            // this resource type does not exist
            if (!_lookup.count(resourceTypeId))
            {
                return nullptr;
            }

            for (auto &resourcePtr : _lookup.at(resourceTypeId))
            {
                if (resourcePtr->id == id)
                {
                    return static_cast<T *>(resourcePtr);
                }
            }
            return nullptr;
        }

        GE_INLINE ShaderStage *loadShaderStage()
        {
            void *const p = appAllocator.allocate(sizeof(ShaderStage), alignof(ShaderStage));
            auto shader = new (p) ShaderStage();
            _lookup[typeId<ShaderStage>()].push_back(shader);
            return shader;
        }

    private:
        std::unordered_map<resourceTypeId, std::pmr::vector<IResource *>> _lookup;
    };
}