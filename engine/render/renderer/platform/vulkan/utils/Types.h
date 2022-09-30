//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TYPES_H
#define SIEGE_ENGINE_TYPES_H

#include <cstdint>
#include <cstdio>

// --------------------------- Pointer Type Forward Declarations ----------------------------------
// VkBuffer <-- pointers

struct VkInstance_T;
struct VkPhysicalDevice_T;
struct VkSurfaceKHR_T;
struct VkDebugUtilsMessengerEXT_T;
struct VkImage_T;
struct VkDeviceMemory_T;
struct VkImageView_T;
struct VkDevice_T;
struct VkQueue_T;
struct VkCommandPool_T;
struct VkCommandBuffer_T;
struct VkDeviceMemory_T;
struct VkBuffer_T;
struct VkSwapchainKHR_T;

typedef VkInstance_T* VkInstance;
typedef VkSurfaceKHR_T* Surface;
typedef VkPhysicalDevice_T* VkPhysicalDevice;
typedef VkDebugUtilsMessengerEXT_T* VkDebugUtilsMessengerEXT;
typedef VkImage_T* VkImage;
typedef VkDeviceMemory_T* VkDeviceMemory;
typedef VkImageView_T* VkImageView;
typedef VkDevice_T* VkDevice;
typedef VkQueue_T* VkQueue;
typedef VkCommandPool_T* VkCommandPool;
typedef VkCommandBuffer_T* CommandBuffer;
typedef VkDeviceMemory_T* VkDeviceMemory;
typedef VkBuffer_T* VkBuffer;
typedef VkSwapchainKHR_T* VkSwapchainKHR;

namespace Siege::Vulkan::Utils
{
// ----------------------------------------- Enums ------------------------------------------------

enum DepthFormat
{
    DEPTH_NONE = 0,
    DEPTH_D32F = 126,
    DEPTH_D24US8UI = 129,
    DEPTH_D32FS8UI = 130,
    DEPTH_D16US8UI = 128,
    DEPTH_D16U = 125
};

enum ImageFormat
{
    NONE = 0,
    RED8UN = 9,
    RED8UI = 13,
    RG8 = 16,
    RGBA = 44,
    BGRA = 50,
    RED16UI = 74,
    RG16F = 83,
    RED32UI = 98,
    RGBA16F = 97,
    RED32F = 100,
    RG32F = 103,
    RGBA32F = 109,
    B10R11G11UF = 122,
    DEPTH32FSTENCIL8UINT = 130,
    DEPTH32F = 126,
    DEPTH24STENCIL8
};

enum ImageUsage
{
    USAGE_NONE = 0,
    USAGE_TEXTURE,
    USAGE_ATTACHMENT,
    USAGE_STORAGE
};

//----------------------------------------- Structs -----------------------------------------------

struct Extent2D
{
    uint32_t width {0};
    uint32_t height {0};
};

struct Extent3D
{
    uint32_t width {0};
    uint32_t height {0};
    uint32_t depth {0};
};

struct Image2DConfig
{
    ImageFormat format;
};

} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_TYPES_H
