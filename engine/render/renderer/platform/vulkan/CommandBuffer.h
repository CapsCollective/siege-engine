//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_COMMANDBUFFER_H
#define SIEGE_ENGINE_COMMANDBUFFER_H

#include "utils/CommandPool.h"

namespace Siege::Vulkan
{

class CommandBuffer
{
public:
    class Pool
    {
    public:
        Pool();
        ~Pool();
    private:
        VkCommandPool commandPool;
    };
    CommandBuffer();
    ~CommandBuffer();
private:
};

} // namespace Siege

#endif // SIEGE_ENGINE_COMMANDBUFFER_H
