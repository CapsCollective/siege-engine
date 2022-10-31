//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Material.h"

#include <utility>

#include "render/renderer/platform/vulkan/utils/Descriptor.h"
#include "render/renderer/descriptor/DescriptorPool.h"
#include "utils/TypeAdaptor.h"
#include "Pipeline.h"

#include <utils/Logging.h>

namespace Siege::Vulkan
{
Material::Material(Shader vertShader, Shader fragShader)
    : vertexShader{std::move(vertShader)}, fragmentShader{std::move(fragShader)}
{
    bufferSize += (Buffer::PadUniformBufferSize(vertexShader.GetTotalUniformSize()) +
                   Buffer::PadUniformBufferSize(fragmentShader.GetTotalUniformSize()));

    // TODO: Make a standalone buffer class
    // Allocate buffer which can store all the data we need
    Buffer::CreateBuffer(bufferSize,
                         VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT buffer.buffer,
                         OUT buffer.bufferMemory);

    // Separate uniforms into unique properties

    uint64_t offset {0};

    AddShader(vertexShader, OUT offset);
    AddShader(fragmentShader, OUT offset);

    // Create Descriptor Set Layout for Sets

    for (size_t i = 0; i < propertiesSlots.Count(); i++)
    {
        auto& set = propertiesSlots[i];

        VkDescriptorSetLayoutBinding bindings[set.properties.Count()];

        for (size_t j = 0; j < set.properties.Count(); j++)
        {
            auto& property = set.properties[j];

            bindings[j] =
                Descriptor::CreateLayoutBinding(j,
                                                1,
                                                property.type,
                                                property.shaderStage);
        }

        CC_ASSERT(Descriptor::CreateLayout(Vulkan::Context::GetVkLogicalDevice(),
                                           OUT set.layout,
                                           bindings,
                                           set.properties.Count()),
                  "Failed to create descriptor set!")

        Descriptor::AllocateSets(Vulkan::Context::GetVkLogicalDevice(),
                                 &set.set,
                                 DescriptorPool::GetDescriptorPool(),
                                 1,
                                 &set.layout);

        VkWriteDescriptorSet writes[set.properties.Count()];

        for (size_t j = 0; j < set.properties.Count(); j++)
        {
            auto& property = set.properties[j];

            VkDescriptorBufferInfo bufferInfo = Descriptor::CreateBufferInfo(buffer.buffer, property.offset, property.size);

            writes[j] = Descriptor::CreateWriteSet(
                j,
                set.set,
                1,
                Utils::ToVkDescriptorType(property.type),
                &bufferInfo);
        }

        Descriptor::WriteSets(Vulkan::Context::GetVkLogicalDevice(),
                              writes,
                              set.properties.Count());
    }

    // Create Pipeline

    ::Siege::Utils::MSArray<VkDescriptorSetLayout, 10> layouts;

    for(size_t i = 0; i < propertiesSlots.Count(); i++) layouts.Append(propertiesSlots[i].layout);

    auto pipeline = Pipeline::Builder()
        .WithRenderPass(Context::GetSwapchain().GetRenderPass())
        .WithDynamicViewport()
        .WithDynamicScissor()
        .WithVertexShader(&vertexShader)
        .WithFragmentShader(&fragmentShader).WithProperties(layouts)
        .WithProperties(layouts)
        .Build();
}

Material::Material(Material&& other) noexcept { Swap(other); }

Material::~Material()
{
    Buffer::DestroyBuffer(buffer);

    for (auto it = propertiesSlots.CreateIterator(); it; ++it)
    {
        vkDestroyDescriptorSetLayout(Vulkan::Context::GetVkLogicalDevice(),
                                     (*it).layout,
                                     nullptr);
    }
}

Material& Material::operator=(Material&& other)
{
    Swap(other);
    return *this;
}

void Material::AddShader(const Shader& shader, uint64_t& offset)
{
    auto uniforms = shader.GetUniforms();

    for (auto it = uniforms.CreateIterator(); it; ++it)
    {
        auto uniform = *it;
        if (uniform.set >= propertiesSlots.Count()) propertiesSlots.Append({});

        auto& slot = propertiesSlots[uniform.set];

        uint32_t propertyIdx = -1;

        for (size_t j = 0; j < slot.properties.Count(); j++)
        {
            if (slot.properties[j].id == uniform.id)
            {
                propertyIdx = j;
                break;
            }
        }

        if (propertyIdx == -1)
        {
            propertyIdx = slot.properties.Count();
            slot.properties.Append({});
        }

        auto& property = slot.properties[propertyIdx];

        property.id = uniform.id;
        property.type = uniform.type;
        property.binding = uniform.slot;
        property.size = uniform.totalSize;
        property.shaderStage = (Utils::ShaderType)(property.shaderStage | shader.GetShaderType());
        property.offset = offset;

        offset += uniform.totalSize;
    }
}

void Material::Swap(Material& other)
{
    auto tmpVertexShader = std::move(vertexShader);
    auto tmpFragmentShader = std::move(fragmentShader);
    auto tmpBufferSize = bufferSize;

    vertexShader = std::move(other.vertexShader);
    fragmentShader = std::move(other.fragmentShader);
    bufferSize = other.bufferSize;

    other.vertexShader = std::move(tmpVertexShader);
    other.fragmentShader = std::move(tmpFragmentShader);
    other.bufferSize = tmpBufferSize;
}
} // namespace Siege::Vulkan