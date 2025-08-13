//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RESOURCEHANDLE_H
#define SIEGE_ENGINE_RESOURCEHANDLE_H

#include <utils/String.h>

namespace Siege
{

template<typename T>
struct ResourceHandle
{
    uint32_t* resourceCount = nullptr;
    T* resourceData = nullptr;

    ResourceHandle() = delete;

    ResourceHandle(uint32_t* count, T* data) : resourceCount(count), resourceData(data)
    {
        ++*resourceCount;
    }

    ResourceHandle(ResourceHandle&& handle) noexcept : resourceCount(handle.resourceCount), resourceData(handle.resourceData)
    {
        ++*resourceCount;
    }

    ~ResourceHandle()
    {
        --*resourceCount;
    }

    T* GetData() const
    {
        return resourceData;
    }

    bool IsValid() const
    {
        return resourceData && *resourceCount > 0;
    }
};

} // namespace Siege

#endif // SIEGE_ENGINE_RESOURCEHANDLE_H
