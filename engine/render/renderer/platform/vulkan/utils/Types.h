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
struct VkSemaphore_T;
struct VkFence_T;
struct VkShaderModule_T;

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
typedef VkCommandBuffer_T* VkCommandBuffer;
typedef VkDeviceMemory_T* VkDeviceMemory;
typedef VkBuffer_T* VkBuffer;
typedef VkSwapchainKHR_T* VkSwapchainKHR;
typedef VkSemaphore_T* VkSemaphore;
typedef VkFence_T* VkFence;
typedef VkShaderModule_T* VkShaderModule;

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

enum Result
{
    SUCCESS = 0,
    NOT_READY = 1,
    TIMEOUT = 2,
    SUBOPTIMAL = 1000001003,
    ERROR_OUT_OF_DATE = -1000001004,
    ERROR_RESIZED = SUBOPTIMAL | ERROR_OUT_OF_DATE,
    ERROR_OUT_OF_HOST_MEM = -1,
    ERROR_OUT_OF_DEVICE_MEM = -2,
    ERROR_DEVICE_LOST = -4
};

enum UniformType
{
    UNKNOWN = 0,
    TEXTURE = 1,
    UNIFORM = 6,
    STORAGE = 7
};

enum ShaderType
{
    EMPTY = 0,
    VERTEX = 0x00000001,
    FRAGMENT = 0x00000010,
    ALL_GRAPHICS = 0x0000001F
};

enum VertexAttributeType
{
    VERTEX_UNDEFINED = 0,
    VERTEX_FLOAT_VEC2 = 103,
    VERTEX_FLOAT_VEC3 = 106,
    VERTEX_FLOAT_VEC4 = 109
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
