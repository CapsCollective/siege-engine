//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Material.h"

#include <utils/Branchless.h>
#include <utils/Logging.h>

#include <utility>

#include "Pipeline.h"
#include "Swapchain.h"
#include "render/renderer/Renderer.h"
#include "render/renderer/descriptor/DescriptorPool.h"
#include "render/renderer/platform/vulkan/utils/Descriptor.h"
#include "utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
Material::Material(Shader vertShader, Shader fragShader) :
    vertexShader {std::move(vertShader)},
    fragmentShader {std::move(fragShader)}
{
    using Vulkan::Context;
    using namespace Vulkan::Utils::Descriptor;

    auto device = Context::GetVkLogicalDevice();

    // Separate uniforms into unique properties

    uint64_t offset {0};

    AddShader(vertexShader, OUT offset);
    AddShader(fragmentShader, OUT offset);

    auto framesCount = Swapchain::MAX_FRAMES_IN_FLIGHT;


    // Allocate buffer which can store all the data we need
    Buffer::CreateBuffer(bufferSize,
                         VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT buffer.buffer,
                         OUT buffer.bufferMemory);

    // Set the number of frames
    perFrameDescriptorSets = MHArray<MSArray<VkDescriptorSet, MAX_UNIFORM_SETS>>(framesCount);

    writes = MHArray<MSArray<VkWriteDescriptorSet, 10>>(framesCount);

    // Create Descriptor Set Layout for Sets
    for (auto slotIt = propertiesSlots.CreateIterator(); slotIt; ++slotIt)
    {
        MSArray<VkDescriptorSetLayoutBinding, 10> bindings;

        auto& slot = *slotIt;

        auto& properties = slot.properties;

        for (auto propIt = properties.CreateIterator(); propIt; ++propIt)
        {
            auto& prop = *propIt;
            bindings.Append(
                CreateLayoutBinding(propIt.GetIndex(), prop.count, prop.type, prop.shaderStage));
            prop.binding = propIt.GetIndex();
        }

        CC_ASSERT(CreateLayout(device, OUT slot.layout, bindings.Data(), properties.Count()),
                  "Failed to create descriptor set!")

        for (auto setIt = perFrameDescriptorSets.CreateFIterator(); setIt; ++setIt)
        {
            auto& sets = *setIt;
            sets.Append(VK_NULL_HANDLE);
            AllocateSets(device,
                         OUT & sets.Back(),
                         DescriptorPool::GetDescriptorPool(),
                         1,
                         &slot.layout);
        }

        WriteSet(slotIt.GetIndex(), slot);
    }

    for (auto writeIt = writes.CreateIterator(); writeIt; ++writeIt) Write(*writeIt);

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

    for (auto it = propertiesSlots.CreateFIterator(); it; ++it)
    {
        vkDestroyDescriptorSetLayout(device, it->layout, nullptr);
    }
}

void Material::Free()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    Buffer::DestroyBuffer(buffer);

    for (auto it = propertiesSlots.CreateFIterator(); it; ++it)
    {
        vkDestroyDescriptorSetLayout(device, it->layout, nullptr);
        it->layout = nullptr;
    }

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

uint32_t Material::SetTexture(Hash::StringId id, Texture2D* texture)
{
    using namespace Utils;
    using namespace Descriptor;

    auto& writeSets = writes[Renderer::GetCurrentFrameIndex()];

    auto texIndex = FindTextureIndex(texture->GetId());

    if (texIndex > -1) return texIndex;

    texIndex = textureIds.Count();
    textureIds.Append(texture->GetId());

    for (auto it = propertiesSlots.CreateIterator(); it; ++it)
    {
        auto& slot = *it;
        auto& properties = slot.properties;

        auto propIdx = FindPropertyIndex(id, slot);

        if (propIdx == -1) continue;

        auto info = texture->GetInfo();

        texture2DInfos[texIndex] = {info.sampler,
                                    info.imageInfo.view,
                                    ToVkImageLayout(info.imageInfo.layout)};

        if (writeSets.Count() > 0) break;

        auto prop = properties[propIdx];

        for (auto write = writes.CreateFIterator(); write; ++write)
        {
            QueueImageUpdate(*write,
                             perFrameDescriptorSets[write.GetIndex()][it.GetIndex()],
                             prop.binding,
                             prop.count,
                             0);
        }
    }

    return texIndex;
}

void Material::AddShader(const Shader& shader, uint64_t& offset)
{
    using Utils::ShaderType;
    using Utils::UniformType;
    using namespace Buffer;

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

        bufferSize +=
            IF(IsStorage(uniform.type) THEN PadStorageBufferSize(uniform.totalSize) ELSE IF(
                IsUniform(uniform.type) THEN PadUniformBufferSize(uniform.totalSize) ELSE 0));

        offset += uniform.totalSize;

        if (IsTexture2D(uniform.type))
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

    if (shader.HasPushConstant())
        pushConstant = {shader.GetPushConstant().size, shader.GetShaderType()};
}

void Material::Bind(const CommandBuffer& commandBuffer)
{
    auto frameIndex = Renderer::GetCurrentFrameIndex();
    graphicsPipeline.Bind(commandBuffer);
    graphicsPipeline.BindSets(commandBuffer, perFrameDescriptorSets[frameIndex]);
}

void Material::BindPushConstant(const CommandBuffer& commandBuffer, const void* values)
{
    graphicsPipeline.PushConstants(commandBuffer, pushConstant.type, pushConstant.size, values);
}

void Material::Recreate()
{
    MSArray<VkDescriptorSetLayout, 10> layouts;

    for (auto it = propertiesSlots.CreateIterator(); it; ++it)
    {
        layouts.Append((*it).layout);
    }

    graphicsPipeline = Pipeline::Builder()
                           .WithRenderPass(Context::GetSwapchain().GetRenderPass())
                           .WithDynamicViewport()
                           .WithDynamicScissor()
                           .WithPushConstant(pushConstant.size, pushConstant.type)
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

    Write(targetSets);
}

void Material::Update(Hash::StringId id)
{
    for (auto it = propertiesSlots.CreateFIterator(); it; ++it)
    {
        auto propertyIndex = FindPropertyIndex(id, *it);

        if (propertyIndex == -1) return;

        auto& setsToWrite = writes[Renderer::GetCurrentFrameIndex()];
        Write(setsToWrite);
    }
}

void Material::WriteSet(uint32_t set, PropertiesSlot& slot)
{
    using namespace Vulkan::Utils;
    using namespace Vulkan::Utils::Descriptor;

    auto& properties = slot.properties;

    for (auto propIt = properties.CreateIterator(); propIt; ++propIt)
    {
        auto prop = *propIt;
        bufferInfos.Append(CreateBufferInfo(buffer.buffer, prop.offset, prop.size));
        for (auto setIt = perFrameDescriptorSets.CreateFIterator(); setIt; ++setIt)
        {
            auto sets = *setIt;
            if (IsTexture2D(prop.type))
                QueueImageUpdate(writes[setIt.GetIndex()], sets[set], propIt.GetIndex());
            else
                QueuePropertyUpdate(writes[setIt.GetIndex()],
                                    sets[set],
                                    prop.type,
                                    propIt.GetIndex(),
                                    1);
        }
    }
}

void Material::Write(MSArray<VkWriteDescriptorSet, 10>& sets)
{
    Utils::Descriptor::WriteSets(Context::GetVkLogicalDevice(), sets.Data(), sets.Count());

    sets.Clear();
}

void Material::QueueImageUpdate(MSArray<VkWriteDescriptorSet, 10>& writeQueue,
                                VkDescriptorSet& set,
                                uint32_t binding,
                                uint32_t count,
                                uint32_t index)
{
    using namespace Vulkan::Utils::Descriptor;

    writeQueue.Append(WriteDescriptorImage(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                           set,
                                           texture2DInfos.Data(),
                                           binding,
                                           count,
                                           index));
}

void Material::QueuePropertyUpdate(MSArray<VkWriteDescriptorSet, 10>& writeQueue,
                                   VkDescriptorSet& set,
                                   Utils::UniformType type,
                                   uint32_t binding,
                                   uint32_t count)
{
    using namespace Vulkan::Utils::Descriptor;

    writeQueue.Append(
        CreateWriteSet(binding, set, count, ToVkDescriptorType(type), &bufferInfos[binding]));
}

int32_t Material::FindPropertyIndex(Hash::StringId id, PropertiesSlot& slot)
{
    int32_t foundIdx {-1};
    for (auto it = slot.properties.CreateIterator(); it; ++it)
    {
        if (it->id == id)
        {
            foundIdx = it.GetIndex();
            break;
        }
    }
    return foundIdx;
}

int32_t Material::FindTextureIndex(Hash::StringId id)
{
    int32_t texExists = -1;
    for (auto it = textureIds.CreateIterator(); it; ++it)
    {
        if (id == *it)
        {
            texExists = it.GetIndex();
            break;
        }
    }
    return texExists;
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
    auto tmpPushConstant = pushConstant;

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
    pushConstant = other.pushConstant;

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
    other.pushConstant = tmpPushConstant;
}
} // namespace Siege::Vulkan
