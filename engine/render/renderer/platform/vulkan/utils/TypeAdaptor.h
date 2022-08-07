//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TYPEADAPTOR_H
#define SIEGE_ENGINE_TYPEADAPTOR_H

#include <volk/volk.h>

#include "Types.h"
#include "../Context.h"

#define DECL_CONVERSION_FUN(to, from, ...)      \
    inline constexpr to To##to(const from& val) \
    {                                           \
        return __VA_ARGS__;                     \
    }

#define SWITCH_MEM(from, name, to) case from::name: return to;
#define SWITCH_DEFAULT(value) default: return value;

#define DECL_VULKAN_SWITCH_FUN(to, from, ...)           \
    inline constexpr to To##to(const from& val)     \
    {                                                   \
        switch(val)                                     \
        {                                               \
            __VA_ARGS__                                 \
        };                                              \
    }

#define GET_MEMBER(name) val.name

namespace Siege::Vulkan::Utils
{
// ----------------------------------------- Enums ------------------------------------------------

DECL_VULKAN_SWITCH_FUN(VkFormat,
                       DepthFormat,
                       SWITCH_MEM(DepthFormat, DEPTH_D32F, VK_FORMAT_D32_SFLOAT)
                       SWITCH_MEM(DepthFormat, DEPTH_D24US8UI, VK_FORMAT_D24_UNORM_S8_UINT)
                       SWITCH_MEM(DepthFormat, DEPTH_D32FS8UI, VK_FORMAT_D32_SFLOAT_S8_UINT)
                       SWITCH_MEM(DepthFormat, DEPTH_D16US8UI, VK_FORMAT_D16_UNORM_S8_UINT)
                       SWITCH_MEM(DepthFormat, DEPTH_D16U, VK_FORMAT_D16_UNORM)
                       SWITCH_DEFAULT(VK_FORMAT_UNDEFINED))

DECL_VULKAN_SWITCH_FUN(DepthFormat,
                       VkFormat ,
                       SWITCH_MEM(VkFormat, VK_FORMAT_D32_SFLOAT, DEPTH_D32F)
                       SWITCH_MEM(VkFormat, VK_FORMAT_D24_UNORM_S8_UINT, DEPTH_D24US8UI)
                       SWITCH_MEM(VkFormat, VK_FORMAT_D32_SFLOAT_S8_UINT, DEPTH_D32FS8UI)
                       SWITCH_MEM(VkFormat, VK_FORMAT_D16_UNORM_S8_UINT, DEPTH_D16US8UI)
                       SWITCH_MEM(VkFormat, VK_FORMAT_D16_UNORM, DEPTH_D16U)
                       SWITCH_DEFAULT(DEPTH_NONE))

DECL_VULKAN_SWITCH_FUN(VkFormat,
                       ImageFormat,
                       SWITCH_MEM(ImageFormat, RED8UN, VK_FORMAT_R8_UNORM)
                       SWITCH_MEM(ImageFormat, RED8UI, VK_FORMAT_R8_UINT)
                       SWITCH_MEM(ImageFormat, RED16UI, VK_FORMAT_R16_UINT)
                       SWITCH_MEM(ImageFormat, RED32UI, VK_FORMAT_R32_UINT)
                       SWITCH_MEM(ImageFormat, RED32F, VK_FORMAT_R32_SFLOAT)
                       SWITCH_MEM(ImageFormat, RG8, VK_FORMAT_R8G8_UNORM)
                       SWITCH_MEM(ImageFormat, RG16F, VK_FORMAT_R16G16_SFLOAT)
                       SWITCH_MEM(ImageFormat, RG32F, VK_FORMAT_R32G32_SFLOAT)
                       SWITCH_MEM(ImageFormat, RGBA, VK_FORMAT_R8G8B8A8_UNORM)
                       SWITCH_MEM(ImageFormat, BGRA, VK_FORMAT_B8G8R8A8_SRGB)
                       SWITCH_MEM(ImageFormat, RGBA16F, VK_FORMAT_R16G16B16A16_SFLOAT)
                       SWITCH_MEM(ImageFormat, RGBA32F, VK_FORMAT_R32G32B32A32_SFLOAT)
                       SWITCH_MEM(ImageFormat, B10R11G11UF, VK_FORMAT_B10G11R11_UFLOAT_PACK32)
                       SWITCH_MEM(ImageFormat, DEPTH32FSTENCIL8UINT, VK_FORMAT_D32_SFLOAT_S8_UINT)
                       SWITCH_MEM(ImageFormat, DEPTH24STENCIL8, VK_FORMAT_R32G32_SFLOAT)
                       SWITCH_DEFAULT(VK_FORMAT_UNDEFINED))

DECL_VULKAN_SWITCH_FUN(ImageFormat,
                       VkFormat ,
                       SWITCH_MEM(VkFormat, VK_FORMAT_R8_UNORM, RED8UN)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R8_UINT, RED8UI)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R16_UINT, RED16UI)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R32_UINT, RED32UI)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R32_SFLOAT, RED32F)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R8G8_UNORM, RG8)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R16G16_SFLOAT, RG16F)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R32G32_SFLOAT, RG32F)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R8G8B8A8_UNORM, RGBA)
                       SWITCH_MEM(VkFormat, VK_FORMAT_B8G8R8A8_SRGB, BGRA)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R16G16B16A16_SFLOAT, RGBA16F)
                       SWITCH_MEM(VkFormat, VK_FORMAT_R32G32B32A32_SFLOAT, RGBA32F)
                       SWITCH_MEM(VkFormat, VK_FORMAT_B10G11R11_UFLOAT_PACK32, B10R11G11UF)
                       SWITCH_MEM(VkFormat, VK_FORMAT_D32_SFLOAT_S8_UINT, DEPTH32FSTENCIL8UINT)
                       SWITCH_MEM(VkFormat, VK_FORMAT_D24_UNORM_S8_UINT, DEPTH24STENCIL8)
                       SWITCH_DEFAULT(NONE))

//----------------------------------------- Structs -----------------------------------------------

DECL_CONVERSION_FUN(VkExtent2D, Extent2D, {GET_MEMBER(width), GET_MEMBER(height)})
DECL_CONVERSION_FUN(Extent2D, VkExtent2D, {GET_MEMBER(width), GET_MEMBER(height)})

DECL_CONVERSION_FUN(VkExtent3D,
                    Extent3D,
                    {GET_MEMBER(width), GET_MEMBER(height), GET_MEMBER(depth)})
DECL_CONVERSION_FUN(Extent3D,
                    VkExtent3D,
                    {GET_MEMBER(width), GET_MEMBER(height), GET_MEMBER(depth)})
} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_TYPEADAPTOR_H
