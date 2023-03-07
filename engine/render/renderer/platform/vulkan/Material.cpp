//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Material.h"

#include <utils/Logging.h>

#include <utility>

#include "Pipeline.h"
#include "Swapchain.h"
#include "render/renderer/Renderer.h"
#include "render/renderer/descriptor/DescriptorPool.h"
#include "render/renderer/platform/vulkan/utils/Descriptor.h"
#include "utils/TypeAdaptor.h"

// TODO(Aryeh): Create a method for adding textures to our texture info array
// TODO(Aryeh): Change the Renderer2D to batch quad calls using our new textures

namespace Siege::Vulkan
{
Material::Material(Shader vertShader, Shader fragShader) :
    vertexShader {std::move(vertShader)},
    fragmentShader {std::move(fragShader)}
{
    // Separate uniforms into unique properties

    uint64_t offset {0};

    AddShader(vertexShader, OUT offset);
    AddShader(fragmentShader, OUT offset);

    auto framesCount = Context::GetSwapchain().MAX_FRAMES_IN_FLIGHT;

    // TODO: Make a standalone buffer class
    // Allocate buffer which can store all the data we need
    Buffer::CreateBuffer(bufferSize,
                         VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT buffer.buffer,
                         OUT buffer.bufferMemory);

    // Set the number of frames
    perFrameDescriptorSets =
        Siege::Utils::MHArray<::Siege::Utils::MSArray<VkDescriptorSet, MAX_UNIFORM_SETS>>(
            framesCount);

    writes = Siege::Utils::MHArray<::Siege::Utils::MSArray<VkWriteDescriptorSet, 10>>(framesCount);

    // Create Descriptor Set Layout for Sets
    propertiesSlots.MForEachI([&](PropertiesSlot& slot, size_t i) {
        ::Siege::Utils::MSArray<VkDescriptorSetLayoutBinding, 10> bindings;

        auto& properties = slot.properties;

        properties.MForEachI([&](Property& prop, size_t j) {
            bindings.Append(
                Descriptor::CreateLayoutBinding(j, prop.count, prop.type, prop.shaderStage));
            prop.binding = j;
        });

        CC_ASSERT(Descriptor::CreateLayout(Vulkan::Context::GetVkLogicalDevice(),
                                           OUT slot.layout,
                                           bindings.Data(),
                                           properties.Count()),
                  "Failed to create descriptor set!")

        perFrameDescriptorSets.ForEach(
            [&](::Siege::Utils::MSArray<VkDescriptorSet, MAX_UNIFORM_SETS>& sets) {
                sets.Append(VK_NULL_HANDLE);
                Descriptor::AllocateSets(Vulkan::Context::GetVkLogicalDevice(),
                                         OUT & sets[sets.Count() - 1],
                                         DescriptorPool::GetDescriptorPool(),
                                         1,
                                         &slot.layout);
            });

        WriteSet(i, slot);
    });

    using Vulkan::Context;

    writes.MForEach([&](::Siege::Utils::MSArray<VkWriteDescriptorSet, 10>& writeSets) {
        Descriptor::WriteSets(Context::GetVkLogicalDevice(), writeSets.Data(), writeSets.Count());
    });

    // Create Pipeline
    Recreate();
}

Material::Material(Material&& other) noexcept
{
    Swap(other);
}

Material::~Material()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();
    if (device == nullptr) return;

    Buffer::DestroyBuffer(buffer);

    propertiesSlots.ForEach(
        [&](PropertiesSlot& prop) { vkDestroyDescriptorSetLayout(device, prop.layout, nullptr); });
}

void Material::Destroy()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    Buffer::DestroyBuffer(buffer);

    propertiesSlots.MForEach([&](PropertiesSlot& prop) {
        vkDestroyDescriptorSetLayout(device, prop.layout, nullptr);
        prop.layout = nullptr;
    });

    vertexShader.Destroy();
    fragmentShader.Destroy();
    graphicsPipeline.Destroy();
}

Material& Material::operator=(Material&& other)
{
    Swap(other);
    return *this;
}

void Material::SetUniformData(Hash::StringId id, uint64_t dataSize, const void* data)
{
    for (auto it = propertiesSlots.CreateIterator(); it; ++it)
    {
        auto slot = *it;

        auto propertyIndex = FindPropertyIndex(id, slot);

        if (propertyIndex == -1) return;

        Buffer::CopyData(buffer, dataSize, data, slot.properties[propertyIndex].offset);
    }
}

void Material::SetTexture(Hash::StringId id, uint32_t index, const Texture2D::Info& textureInfo)
{
    auto& writeSets = writes[Renderer::GetCurrentFrameIndex()];
    auto& descriptors = perFrameDescriptorSets[Renderer::GetCurrentFrameIndex()];

    propertiesSlots.MForEachI([&](PropertiesSlot& slot, size_t i) {
        auto& properties = slot.properties;
        auto propIdx = FindPropertyIndex(id, slot);

        if (propIdx == -1) return;

        texture2DInfos[index] = {textureInfo.sampler,
                                 textureInfo.imageInfo.view,
                                 Utils::ToVkImageLayout(textureInfo.imageInfo.layout)};

        if (writeSets.Count() > 0) return;

        auto prop = properties[propIdx];

        writeSets.Append(Descriptor::WriteDescriptorImage(Utils::ToVkDescriptorType(prop.type),
                                                          descriptors[i],
                                                          texture2DInfos.Data(),
                                                          prop.binding,
                                                          prop.count,
                                                          0));
    });
}

void Material::AddShader(const Shader& shader, uint64_t& offset)
{
    using Utils::ShaderType;
    auto uniforms = shader.GetUniforms();

    for (auto it = uniforms.CreateIterator(); it; ++it)
    {
        auto uniform = *it;
        if (uniform.set >= propertiesSlots.Count()) propertiesSlots.Append({});

        int32_t propertyIndex = FindPropertyIndex(uniform.id, propertiesSlots[uniform.set]);
        auto& properties = propertiesSlots[uniform.set].properties;

        if (propertyIndex > -1)
        {
            auto& prop = properties[propertyIndex];
            prop.shaderStage = (ShaderType) (prop.shaderStage | shader.GetShaderType());
            return;
        }

        properties.Append({
            uniform.id,
            static_cast<uint32_t>(properties.Count()),
            uniform.count,
            offset,
            uniform.totalSize,
            uniform.type,
            shader.GetShaderType(),
        });

        using Vulkan::Utils::UniformType;

        bufferSize += ((uniform.type == UniformType::STORAGE) *
                       Buffer::PadStorageBufferSize(uniform.totalSize)) +
                      ((uniform.type == UniformType::UNIFORM) *
                       Buffer::PadUniformBufferSize(uniform.totalSize));
        offset += uniform.totalSize;

        if (uniform.type == Utils::TEXTURE2D)
        {
            auto& defaultTexture2DInfo = shader.GetDefaultTexture2DInfo();
            for (size_t i = 0; i < uniform.count; i++)
            {
                texture2DInfos.Append(
                    {defaultTexture2DInfo.sampler,
                     defaultTexture2DInfo.imageInfo.view,
                     Utils::ToVkImageLayout(defaultTexture2DInfo.imageInfo.layout)});
            }
        }
    }
}

void Material::Bind(const CommandBuffer& commandBuffer)
{
    graphicsPipeline.Bind(commandBuffer);
    graphicsPipeline.BindSets(commandBuffer,
                              perFrameDescriptorSets[Renderer::GetCurrentFrameIndex()]);
}

void Material::Recreate()
{
    ::Siege::Utils::MSArray<VkDescriptorSetLayout, 10> layouts;

    propertiesSlots.MForEach([&](PropertiesSlot& slot) { layouts.Append(slot.layout); });

    graphicsPipeline = Pipeline::Builder()
                           .WithRenderPass(Context::GetSwapchain().GetRenderPass())
                           .WithDynamicViewport()
                           .WithDynamicScissor()
                           .WithVertexShader(&vertexShader)
                           .WithFragmentShader(&fragmentShader)
                           .WithProperties(layouts)
                           .Build();
}

void Material::Update()
{
    auto currentFrameIdx = Renderer::GetCurrentFrameIndex();

    auto& targetSets = writes[currentFrameIdx];

    using Vulkan::Context;

    Descriptor::WriteSets(Context::GetVkLogicalDevice(), targetSets.Data(), targetSets.Count());

    targetSets.Clear();
}

void Material::Update(Hash::StringId id)
{
    propertiesSlots.ForEachI([&](PropertiesSlot& slot, size_t i) {
        auto propertyIndex = FindPropertyIndex(id, slot);

        if (propertyIndex == -1) return;

        auto& setsToWrite = writes[Renderer::GetCurrentFrameIndex()];
        Descriptor::WriteSets(Context::GetVkLogicalDevice(), &setsToWrite[propertyIndex], 1);
    });
}

void Material::WriteSet(uint32_t set, PropertiesSlot& slot)
{
    auto& properties = slot.properties;

    properties.MForEachI([&](Property& prop, size_t i) {
        bufferInfos.Append(Descriptor::CreateBufferInfo(buffer.buffer, prop.offset, prop.size));
        perFrameDescriptorSets.ForEachI(
            [&](::Siege::Utils::MSArray<VkDescriptorSet, MAX_UNIFORM_SETS>& sets, size_t j) {
                if (prop.type == Utils::TEXTURE2D)
                {
                    writes[j].Append(
                        Descriptor::WriteDescriptorImage(Utils::ToVkDescriptorType(prop.type),
                                                         sets[set],
                                                         texture2DInfos.Data(),
                                                         i,
                                                         MAX_TEXTURES,
                                                         0));
                }
                else
                {
                    writes[j].Append(
                        Descriptor::CreateWriteSet(i,
                                                   sets[set],
                                                   1,
                                                   Utils::ToVkDescriptorType(prop.type),
                                                   &bufferInfos[i]));
                }
            });
    });
}

int32_t Material::FindPropertyIndex(Hash::StringId id, PropertiesSlot& slot)
{
    int32_t foundIdx {-1};
    slot.properties.MForEachI([&](Property& property, size_t i){
        if (property.id == id)
        {
            foundIdx = i;
            return;
        }
    });
    return foundIdx;
}

void Material::Swap(Material& other)
{
    auto tmpVertexShader = std::move(vertexShader);
    auto tmpFragmentShader = std::move(fragmentShader);
    auto tmpBufferSize = bufferSize;
    auto tmpBuffer = buffer;
    auto tmpGraphicsPipeline = std::move(graphicsPipeline);
    auto tmpPropertiesSlots = propertiesSlots;
    auto tmpPerFrameDescriptors = std::move(perFrameDescriptorSets);
    auto tmpWrites = std::move(writes);
    auto tmpBufferInfos = std::move(bufferInfos);
    auto tmpTexture2DInfos = std::move(texture2DInfos);

    vertexShader = std::move(other.vertexShader);
    fragmentShader = std::move(other.fragmentShader);
    bufferSize = other.bufferSize;
    buffer = other.buffer;
    graphicsPipeline = std::move(other.graphicsPipeline);
    propertiesSlots = other.propertiesSlots;
    perFrameDescriptorSets = std::move(other.perFrameDescriptorSets);
    writes = std::move(other.writes);
    bufferInfos = std::move(other.bufferInfos);
    texture2DInfos = std::move(other.texture2DInfos);

    other.vertexShader = std::move(tmpVertexShader);
    other.fragmentShader = std::move(tmpFragmentShader);
    other.bufferSize = tmpBufferSize;
    other.buffer = tmpBuffer;
    other.graphicsPipeline = std::move(tmpGraphicsPipeline);
    other.propertiesSlots = tmpPropertiesSlots;
    other.perFrameDescriptorSets = std::move(tmpPerFrameDescriptors);
    other.writes = std::move(tmpWrites);
    other.bufferInfos = std::move(tmpBufferInfos);
    other.texture2DInfos = std::move(tmpTexture2DInfos);

    // Need to reset our pointer references in the writes array (if there are any)
    propertiesSlots.MForEachI([&](PropertiesSlot& slot, size_t i) {
        slot.properties.MForEachI([&](Property& prop, size_t j) {
            writes.MForEach([&](::Siege::Utils::MSArray<VkWriteDescriptorSet, 10>& writeSets) {
                if (prop.type == Utils::TEXTURE2D) writeSets[j].pImageInfo = texture2DInfos.Data();
                else writeSets[j].pBufferInfo = &bufferInfos[j];
            });
        });
    });
}
} // namespace Siege::Vulkan
