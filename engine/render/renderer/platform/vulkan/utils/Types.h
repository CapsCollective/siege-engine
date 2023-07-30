//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
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
struct VkPipelineLayout_T;
struct VkPipeline_T;
struct VkSampler_T;
struct VkDescriptorPool_T;
struct VkRenderPass_T;
struct VkFramebuffer_T;
struct VkDescriptorSet_T;
struct VkDescriptorSetLayout_T;
struct VkWriteDescriptorSet;

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
typedef VkPipelineLayout_T* VkPipelineLayout;
typedef VkPipeline_T* VkPipeline;
typedef VkSampler_T* VkSampler;
typedef VkFramebuffer_T* VkFramebuffer;
typedef VkDescriptorSet_T* VkDescriptorSet;
typedef VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Siege::Vulkan::Utils
{
typedef VkDescriptorPool_T* UniformAllocator;
typedef VkRenderPass_T* RenderPass;
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
    RGBASRGB = 43,
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
    DEPTH24STENCIL8,
    DEPTH24US8UI = 129,
    DEPTH32FS8UI = 130,
    DEPTH16US8UI = 128,
    DEPTH16U = 125
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
    TEXTURE2D = 1,
    UNIFORM = 6,
    STORAGE = 7,
    UNIFORM_DYNAMIC = 8,
    STORAGE_DYNAMIC = 9
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
    VERTEX_UINT_32 = 98,
    VERTEX_FLOAT_VEC2 = 103,
    VERTEX_FLOAT_VEC3 = 106,
    VERTEX_FLOAT_VEC4 = 109
};

enum PipelineTopology
{
    TOPOLOGY_POINT_LIST = 0,
    TOPOLOGY_LINE_LIST = 1,
    TOPOLOGY_TRIANGLE_LIST = 3
};

enum ImageLayout
{
    LAYOUT_UNDEFINED = 0,
    LAYOUT_COLOUR_ATTACHMENT = 2,
    LAYOUT_DEPTH_STENCIL_ATTACHMENT = 3,
    LAYOUT_SHADER_READ_ONLY_OPTIMAL = 5,
    LAYOUT_TRANSFER_DST_OPTIMAL = 7,
};

enum BufferType
{
    TRANSFER_SRC_BUFFER = 0x00000001,
    TRANSFER_DST_BUFFER = 0x00000002,
    UNIFORM_BUFFER = 0x00000010,
    STORAGE_BUFFER = 0x00000020,
    INDEX_BUFFER = 0x00000040,
    VERTEX_BUFFER = 0x00000080,
};

enum PipelineStage
{
    STAGE_NONE = 0,
    STAGE_TOP_OF_PIPE = 0x00000001,
    STAGE_VERTEX_SHADER = 0x00000008,
    STAGE_FRAGMENT_SHADER = 0x00000080,
    STAGE_EARLY_FRAGMENT_TEST = 0x00000100,
    STAGE_COLOUR_ATTACHMENT_OUTPUT = 0x00000400,
    STAGE_TRANSFER_BIT = 0x00001000,
};

enum MemoryAccess
{
    ACCESS_NONE = 0,
    ACCESS_TRANSFER_WRITE = 0x00001000,
    ACCESS_TRANSFER_READ = 0x00000800,
    ACCESS_SHADER_READ = 0x00000020,
    ACCESS_SHADER_WRITE = 0x00000040,
    ACCESS_COLOUR_ATTACHMENT_WRITE = 0x00000100,
    ACCESS_DEPTH_ATTACHMENT_WRITE = 0x00000400,
};

enum ImageAspect
{
    ASPECT_NONE = 0,
    ASPECT_COLOUR_BIT = 0x00000001,
    ASPECT_DEPTH_BIT = 0x00000002,
    ASPECT_STENCIL_BIT = 0x00000004
};

enum VertexInputRate
{
    INPUT_RATE_VERTEX = 0,
    INPUT_RATE_INSTANCE = 1
};

enum MemoryProperty
{
    MEMORY_DEVICE_LOCAL = 0x00000001,
    MEMORY_HOST_COHERENT = 0x00000002,
    MEMORY_HOST_VISIBLE = 0x00000004
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

struct Offset3D
{
    int32_t width {0};
    int32_t height {0};
    int32_t depth {0};
};

struct Offset2D
{
    int32_t width {0};
    int32_t height {0};
};

struct Extent2DF
{
    float width {0};
    float height {0};
};

struct Image2DConfig
{
    ImageFormat format;
};

struct UniformAllocation
{
    UniformType type;
    uint32_t uniformCount;
};

} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_TYPES_H
