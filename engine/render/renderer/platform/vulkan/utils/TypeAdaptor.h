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

#include "../Context.h"
#include "Types.h"

#define DECL_CONVERSION_FUN(to, from, ...)      \
    inline constexpr to To##to(const from& val) \
    {                                           \
        return __VA_ARGS__;                     \
    }

#define SWITCH_MEM(from, name, to) \
    case from::name:               \
        return to;
#define SWITCH_DEFAULT(value) \
    default:                  \
        return value;

#define DECL_VULKAN_SWITCH_FUN(to, from, ...)   \
    inline constexpr to To##to(const from& val) \
    {                                           \
        switch (val)                            \
        {                                       \
            __VA_ARGS__                         \
        };                                      \
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
                                   SWITCH_MEM(DepthFormat,
                                              DEPTH_D16US8UI,
                                              VK_FORMAT_D16_UNORM_S8_UINT)
                                       SWITCH_MEM(DepthFormat, DEPTH_D16U, VK_FORMAT_D16_UNORM)
                                           SWITCH_DEFAULT(VK_FORMAT_UNDEFINED))

DECL_VULKAN_SWITCH_FUN(DepthFormat,
                       VkFormat,
                       SWITCH_MEM(VkFormat, VK_FORMAT_D32_SFLOAT, DEPTH_D32F)
                           SWITCH_MEM(VkFormat, VK_FORMAT_D24_UNORM_S8_UINT, DEPTH_D24US8UI)
                               SWITCH_MEM(VkFormat, VK_FORMAT_D32_SFLOAT_S8_UINT, DEPTH_D32FS8UI)
                                   SWITCH_MEM(VkFormat, VK_FORMAT_D16_UNORM_S8_UINT, DEPTH_D16US8UI)
                                       SWITCH_MEM(VkFormat, VK_FORMAT_D16_UNORM, DEPTH_D16U)
                                           SWITCH_DEFAULT(DEPTH_NONE))

DECL_VULKAN_SWITCH_FUN(
    VkFormat,
    ImageFormat,
    SWITCH_MEM(ImageFormat, RED8UN, VK_FORMAT_R8_UNORM)
        SWITCH_MEM(ImageFormat, RED8UI, VK_FORMAT_R8_UINT)
            SWITCH_MEM(ImageFormat, RED16UI, VK_FORMAT_R16_UINT) SWITCH_MEM(ImageFormat,
                                                                            RED32UI,
                                                                            VK_FORMAT_R32_UINT)
                SWITCH_MEM(ImageFormat, RED32F, VK_FORMAT_R32_SFLOAT)
                    SWITCH_MEM(ImageFormat, RG8, VK_FORMAT_R8G8_UNORM)
                        SWITCH_MEM(ImageFormat, RGBASRGB, VK_FORMAT_R8G8B8A8_SRGB)
                            SWITCH_MEM(ImageFormat, RG16F, VK_FORMAT_R16G16_SFLOAT)
                                SWITCH_MEM(ImageFormat, RG32F, VK_FORMAT_R32G32_SFLOAT)
                                    SWITCH_MEM(ImageFormat, RGBA, VK_FORMAT_R8G8B8A8_UNORM)
                                        SWITCH_MEM(ImageFormat, BGRA, VK_FORMAT_B8G8R8A8_SRGB)
                                            SWITCH_MEM(ImageFormat,
                                                       RGBA16F,
                                                       VK_FORMAT_R16G16B16A16_SFLOAT)
                                                SWITCH_MEM(ImageFormat,
                                                           RGBA32F,
                                                           VK_FORMAT_R32G32B32A32_SFLOAT)
                                                    SWITCH_MEM(ImageFormat,
                                                               B10R11G11UF,
                                                               VK_FORMAT_B10G11R11_UFLOAT_PACK32)
                                                        SWITCH_MEM(ImageFormat,
                                                                   DEPTH32FSTENCIL8UINT,
                                                                   VK_FORMAT_D32_SFLOAT_S8_UINT)
                                                            SWITCH_MEM(ImageFormat,
                                                                       DEPTH24STENCIL8,
                                                                       VK_FORMAT_R32G32_SFLOAT)
                                                                SWITCH_DEFAULT(VK_FORMAT_UNDEFINED))

DECL_VULKAN_SWITCH_FUN(
    ImageFormat,
    VkFormat,
    SWITCH_MEM(VkFormat, VK_FORMAT_R8_UNORM, RED8UN) SWITCH_MEM(VkFormat, VK_FORMAT_R8_UINT, RED8UI)
        SWITCH_MEM(VkFormat, VK_FORMAT_R16_UINT, RED16UI)
            SWITCH_MEM(VkFormat, VK_FORMAT_R32_UINT, RED32UI) SWITCH_MEM(VkFormat,
                                                                         VK_FORMAT_R32_SFLOAT,
                                                                         RED32F)
                SWITCH_MEM(VkFormat, VK_FORMAT_R8G8_UNORM, RG8) SWITCH_MEM(VkFormat,
                                                                           VK_FORMAT_R8G8B8A8_SRGB,
                                                                           RGBASRGB)
                    SWITCH_MEM(VkFormat, VK_FORMAT_R16G16_SFLOAT, RG16F)
                        SWITCH_MEM(VkFormat, VK_FORMAT_R32G32_SFLOAT, RG32F)
                            SWITCH_MEM(VkFormat, VK_FORMAT_R8G8B8A8_UNORM, RGBA)
                                SWITCH_MEM(VkFormat, VK_FORMAT_B8G8R8A8_SRGB, BGRA)
                                    SWITCH_MEM(VkFormat, VK_FORMAT_R16G16B16A16_SFLOAT, RGBA16F)
                                        SWITCH_MEM(VkFormat, VK_FORMAT_R32G32B32A32_SFLOAT, RGBA32F)
                                            SWITCH_MEM(VkFormat,
                                                       VK_FORMAT_B10G11R11_UFLOAT_PACK32,
                                                       B10R11G11UF)
                                                SWITCH_MEM(VkFormat,
                                                           VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                           DEPTH32FSTENCIL8UINT)
                                                    SWITCH_MEM(VkFormat,
                                                               VK_FORMAT_D24_UNORM_S8_UINT,
                                                               DEPTH24STENCIL8)
                                                        SWITCH_DEFAULT(NONE))

DECL_VULKAN_SWITCH_FUN(VkDescriptorType,
                       UniformType,
                       SWITCH_MEM(UniformType, TEXTURE2D, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                           SWITCH_MEM(UniformType, STORAGE, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
                               SWITCH_MEM(UniformType, UNIFORM, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                                   SWITCH_DEFAULT(VK_DESCRIPTOR_TYPE_MAX_ENUM))

DECL_VULKAN_SWITCH_FUN(
    UniformType,
    VkDescriptorType,
    SWITCH_MEM(VkDescriptorType, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, TEXTURE2D)
        SWITCH_MEM(VkDescriptorType, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, STORAGE)
            SWITCH_MEM(VkDescriptorType, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, UNIFORM)
                SWITCH_DEFAULT(UNKNOWN))

DECL_VULKAN_SWITCH_FUN(VkShaderStageFlagBits,
                       ShaderType,
                       SWITCH_MEM(ShaderType, VERTEX, VK_SHADER_STAGE_VERTEX_BIT)
                           SWITCH_MEM(ShaderType, FRAGMENT, VK_SHADER_STAGE_FRAGMENT_BIT)
                               SWITCH_DEFAULT(VK_SHADER_STAGE_ALL_GRAPHICS))

DECL_VULKAN_SWITCH_FUN(ShaderType,
                       VkShaderStageFlagBits,
                       SWITCH_MEM(VkShaderStageFlagBits, VK_SHADER_STAGE_VERTEX_BIT, VERTEX)
                           SWITCH_MEM(VkShaderStageFlagBits, VK_SHADER_STAGE_FRAGMENT_BIT, FRAGMENT)
                               SWITCH_DEFAULT(ALL_GRAPHICS))

DECL_VULKAN_SWITCH_FUN(
    VkFormat,
    VertexAttributeType,
    SWITCH_MEM(VertexAttributeType, VERTEX_FLOAT_VEC2, VK_FORMAT_R32G32_SFLOAT)
        SWITCH_MEM(VertexAttributeType, VERTEX_UINT_32, VK_FORMAT_R32_UINT)
            SWITCH_MEM(VertexAttributeType, VERTEX_FLOAT_VEC3, VK_FORMAT_R32G32B32_SFLOAT)
                SWITCH_MEM(VertexAttributeType, VERTEX_FLOAT_VEC4, VK_FORMAT_R32G32B32A32_SFLOAT)
                    SWITCH_DEFAULT(VK_FORMAT_UNDEFINED))

DECL_VULKAN_SWITCH_FUN(VertexAttributeType,
                       VkFormat,
                       SWITCH_MEM(VkFormat, VK_FORMAT_R32G32_SFLOAT, VERTEX_FLOAT_VEC2)
                           SWITCH_MEM(VkFormat, VK_FORMAT_R32_UINT, VERTEX_UINT_32)
                               SWITCH_MEM(VkFormat, VK_FORMAT_R32G32B32_SFLOAT, VERTEX_FLOAT_VEC3)
                                   SWITCH_MEM(VkFormat,
                                              VK_FORMAT_R32G32B32A32_SFLOAT,
                                              VERTEX_FLOAT_VEC4) SWITCH_DEFAULT(VERTEX_UNDEFINED))

DECL_VULKAN_SWITCH_FUN(
    PipelineTopology,
    VkPrimitiveTopology,
    SWITCH_MEM(VkPrimitiveTopology, VK_PRIMITIVE_TOPOLOGY_POINT_LIST, TOPOLOGY_POINT_LIST)
        SWITCH_MEM(VkPrimitiveTopology, VK_PRIMITIVE_TOPOLOGY_LINE_LIST, TOPOLOGY_LINE_LIST)
            SWITCH_MEM(VkPrimitiveTopology,
                       VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                       TOPOLOGY_TRIANGLE_LIST) SWITCH_DEFAULT(TOPOLOGY_POINT_LIST))

DECL_VULKAN_SWITCH_FUN(
    VkPrimitiveTopology,
    PipelineTopology,
    SWITCH_MEM(PipelineTopology, TOPOLOGY_POINT_LIST, VK_PRIMITIVE_TOPOLOGY_POINT_LIST)
        SWITCH_MEM(PipelineTopology, TOPOLOGY_LINE_LIST, VK_PRIMITIVE_TOPOLOGY_LINE_LIST)
            SWITCH_MEM(PipelineTopology,
                       TOPOLOGY_TRIANGLE_LIST,
                       VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
                SWITCH_DEFAULT(VK_PRIMITIVE_TOPOLOGY_POINT_LIST))

DECL_VULKAN_SWITCH_FUN(
    ImageLayout,
    VkImageLayout,
    SWITCH_MEM(VkImageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, LAYOUT_TRANSFER_DST_OPTIMAL)
        SWITCH_MEM(VkImageLayout,
                   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                   LAYOUT_COLOUR_ATTACHMENT)
            SWITCH_MEM(VkImageLayout,
                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                       LAYOUT_DEPTH_STENCIL_ATTACHMENT)
                SWITCH_MEM(VkImageLayout,
                           VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                           LAYOUT_SHADER_READ_ONLY_OPTIMAL) SWITCH_DEFAULT(LAYOUT_UNDEFINED))

DECL_VULKAN_SWITCH_FUN(
    VkImageLayout,
    ImageLayout,
    SWITCH_MEM(ImageLayout, LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        SWITCH_MEM(ImageLayout, LAYOUT_COLOUR_ATTACHMENT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
            SWITCH_MEM(ImageLayout,
                       LAYOUT_DEPTH_STENCIL_ATTACHMENT,
                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
                SWITCH_MEM(ImageLayout,
                           LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                           VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
                    SWITCH_DEFAULT(VK_IMAGE_LAYOUT_UNDEFINED))

DECL_VULKAN_SWITCH_FUN(
    BufferType,
    VkBufferUsageFlagBits,
    SWITCH_MEM(VkBufferUsageFlagBits, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, TRANSFER_SRC_BUFFER)
        SWITCH_MEM(VkBufferUsageFlagBits, VK_BUFFER_USAGE_TRANSFER_DST_BIT, TRANSFER_DST_BUFFER)
            SWITCH_MEM(VkBufferUsageFlagBits, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, INDEX_BUFFER)
                SWITCH_MEM(VkBufferUsageFlagBits, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VERTEX_BUFFER)
                    SWITCH_MEM(VkBufferUsageFlagBits,
                               VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                               UNIFORM_BUFFER) SWITCH_MEM(VkBufferUsageFlagBits,
                                                          VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                                          STORAGE_BUFFER)
                        SWITCH_DEFAULT(TRANSFER_SRC_BUFFER))
DECL_VULKAN_SWITCH_FUN(
    VkBufferUsageFlagBits,
    BufferType,
    SWITCH_MEM(BufferType, TRANSFER_SRC_BUFFER, VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
        SWITCH_MEM(BufferType, TRANSFER_DST_BUFFER, VK_BUFFER_USAGE_TRANSFER_DST_BIT)
            SWITCH_MEM(BufferType, INDEX_BUFFER, VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
                SWITCH_MEM(BufferType, VERTEX_BUFFER, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
                    SWITCH_MEM(BufferType, UNIFORM_BUFFER, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
                        SWITCH_MEM(BufferType, STORAGE_BUFFER, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
                            SWITCH_DEFAULT(VK_BUFFER_USAGE_TRANSFER_SRC_BIT))

DECL_VULKAN_SWITCH_FUN(MemoryProperty,
                       VkMemoryPropertyFlagBits,
                       SWITCH_MEM(VkMemoryPropertyFlagBits,
                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                  MEMORY_DEVICE_LOCAL)
                           SWITCH_MEM(VkMemoryPropertyFlagBits,
                                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                      MEMORY_HOST_COHERENT)
                               SWITCH_MEM(VkMemoryPropertyFlagBits,
                                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                                          MEMORY_HOST_VISIBLE) SWITCH_DEFAULT(MEMORY_DEVICE_LOCAL))

DECL_VULKAN_SWITCH_FUN(
    VkMemoryPropertyFlagBits,
    MemoryProperty,
    SWITCH_MEM(MemoryProperty, MEMORY_DEVICE_LOCAL, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        SWITCH_MEM(MemoryProperty, MEMORY_HOST_COHERENT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
            SWITCH_MEM(MemoryProperty, MEMORY_HOST_VISIBLE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))

DECL_VULKAN_SWITCH_FUN(PipelineStage,
                       VkPipelineStageFlagBits,
                       SWITCH_MEM(VkPipelineStageFlagBits,
                                  VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                  STAGE_TOP_OF_PIPE) SWITCH_MEM(VkPipelineStageFlagBits,
                                                                VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
                                                                STAGE_VERTEX_SHADER)
                           SWITCH_MEM(VkPipelineStageFlagBits,
                                      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                      STAGE_FRAGMENT_SHADER)
                               SWITCH_MEM(VkPipelineStageFlagBits,
                                          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                          STAGE_COLOUR_ATTACHMENT_OUTPUT)
                                   SWITCH_MEM(VkPipelineStageFlagBits,
                                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                                              STAGE_EARLY_FRAGMENT_TEST)
                                       SWITCH_MEM(VkPipelineStageFlagBits,
                                                  VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                  STAGE_TRANSFER_BIT) SWITCH_DEFAULT(STAGE_NONE))

DECL_VULKAN_SWITCH_FUN(
    VkPipelineStageFlagBits,
    PipelineStage,
    SWITCH_MEM(PipelineStage, STAGE_TOP_OF_PIPE, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT)
        SWITCH_MEM(PipelineStage, STAGE_VERTEX_SHADER, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT)
            SWITCH_MEM(PipelineStage,
                       STAGE_COLOUR_ATTACHMENT_OUTPUT,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
                SWITCH_MEM(PipelineStage,
                           STAGE_EARLY_FRAGMENT_TEST,
                           VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
                    SWITCH_MEM(PipelineStage,
                               STAGE_FRAGMENT_SHADER,
                               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)
                        SWITCH_MEM(PipelineStage,
                                   STAGE_TRANSFER_BIT,
                                   VK_PIPELINE_STAGE_TRANSFER_BIT)
                            SWITCH_DEFAULT(VK_PIPELINE_STAGE_NONE))

DECL_VULKAN_SWITCH_FUN(
    VkAccessFlagBits,
    MemoryAccess,
    SWITCH_MEM(MemoryAccess, ACCESS_SHADER_READ, VK_ACCESS_SHADER_READ_BIT)
        SWITCH_MEM(MemoryAccess, ACCESS_SHADER_WRITE, VK_ACCESS_SHADER_WRITE_BIT)
            SWITCH_MEM(MemoryAccess, ACCESS_TRANSFER_READ, VK_ACCESS_TRANSFER_READ_BIT)
                SWITCH_MEM(MemoryAccess, ACCESS_TRANSFER_WRITE, VK_ACCESS_TRANSFER_WRITE_BIT)
                    SWITCH_MEM(MemoryAccess,
                               ACCESS_COLOUR_ATTACHMENT_WRITE,
                               VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
                        SWITCH_MEM(MemoryAccess,
                                   ACCESS_DEPTH_ATTACHMENT_WRITE,
                                   VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)
                            SWITCH_DEFAULT(VK_ACCESS_NONE))

DECL_VULKAN_SWITCH_FUN(
    MemoryAccess,
    VkAccessFlagBits,
    SWITCH_MEM(VkAccessFlagBits, VK_ACCESS_SHADER_READ_BIT, ACCESS_SHADER_READ)
        SWITCH_MEM(VkAccessFlagBits, VK_ACCESS_SHADER_WRITE_BIT, ACCESS_SHADER_WRITE)
            SWITCH_MEM(VkAccessFlagBits, VK_ACCESS_TRANSFER_READ_BIT, ACCESS_TRANSFER_READ)
                SWITCH_MEM(VkAccessFlagBits, VK_ACCESS_TRANSFER_WRITE_BIT, ACCESS_TRANSFER_WRITE)
                    SWITCH_MEM(VkAccessFlagBits,
                               VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                               ACCESS_COLOUR_ATTACHMENT_WRITE)
                        SWITCH_MEM(VkAccessFlagBits,
                                   VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                                   ACCESS_DEPTH_ATTACHMENT_WRITE) SWITCH_DEFAULT(ACCESS_NONE))

DECL_VULKAN_SWITCH_FUN(
    ImageAspect,
    VkImageAspectFlagBits,
    SWITCH_MEM(VkImageAspectFlagBits, VK_IMAGE_ASPECT_COLOR_BIT, ASPECT_COLOUR_BIT)
        SWITCH_MEM(VkImageAspectFlagBits, VK_IMAGE_ASPECT_DEPTH_BIT, ASPECT_DEPTH_BIT)
            SWITCH_MEM(VkImageAspectFlagBits, VK_IMAGE_ASPECT_STENCIL_BIT, ASPECT_STENCIL_BIT)
                SWITCH_DEFAULT(ASPECT_NONE))

DECL_VULKAN_SWITCH_FUN(VkImageAspectFlagBits,
                       ImageAspect,
                       SWITCH_MEM(ImageAspect, ASPECT_COLOUR_BIT, VK_IMAGE_ASPECT_COLOR_BIT)
                           SWITCH_MEM(ImageAspect, ASPECT_DEPTH_BIT, VK_IMAGE_ASPECT_DEPTH_BIT)
                               SWITCH_MEM(ImageAspect,
                                          ASPECT_STENCIL_BIT,
                                          VK_IMAGE_ASPECT_STENCIL_BIT)
                                   SWITCH_DEFAULT(VK_IMAGE_ASPECT_NONE))

DECL_VULKAN_SWITCH_FUN(VkVertexInputRate,
                       VertexInputRate,
                       SWITCH_MEM(VertexInputRate,
                                  INPUT_RATE_INSTANCE,
                                  VK_VERTEX_INPUT_RATE_INSTANCE)
                           SWITCH_DEFAULT(VK_VERTEX_INPUT_RATE_VERTEX))

DECL_VULKAN_SWITCH_FUN(VertexInputRate,
                       VkVertexInputRate,
                       SWITCH_MEM(VkVertexInputRate,
                                  VK_VERTEX_INPUT_RATE_INSTANCE,
                                  INPUT_RATE_INSTANCE) SWITCH_DEFAULT(INPUT_RATE_VERTEX))

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
