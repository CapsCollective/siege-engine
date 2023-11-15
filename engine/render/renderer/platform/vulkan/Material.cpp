//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Material.h"

#include <utils/Branchless.h>
#include <utils/Logging.h>

#include <utility>

#include "Pipeline.h"
#include "Swapchain.h"
#include "render/renderer/Renderer.h"
#include "render/renderer/platform/vulkan/DescriptorPool.h"
#include "render/renderer/platform/vulkan/utils/Descriptor.h"
#include "utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
Material::Material(Shader vertShader, Shader fragShader, bool isWritingDepth) :
    vertexShader {std::move(vertShader)},
    fragmentShader {std::move(fragShader)},
    isWritingDepth {isWritingDepth}
{
    using Vulkan::Context;
    using namespace Vulkan::Utils::Descriptor;

    auto device = Context::GetVkLogicalDevice();

    const auto framesCount = Swapchain::MAX_FRAMES_IN_FLIGHT;

    // Separate uniforms into unique properties

    bufferUpdates = MHArray<MHArray<UniformBufferUpdate>>(framesCount);
    imageUpdates = MHArray<MHArray<UniformImageUpdate>>(framesCount);

    for (size_t i = 0; i < framesCount; i++)
    {
        bufferUpdates.Append(MHArray<UniformBufferUpdate>(10));
        imageUpdates.Append(MHArray<UniformImageUpdate>(MAX_TEXTURES));
    }

    textureInfos = MHArray<ImageData>(MAX_TEXTURES);

    uint64_t offset {0};

    AddShader(vertexShader, OUT offset);
    AddShader(fragmentShader, OUT offset);

    // Allocate buffer which can store all the data we need
    Buffer::CreateBuffer(bufferSize,
                         Vulkan::Utils::STORAGE_BUFFER | Vulkan::Utils::UNIFORM_BUFFER,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT buffer.buffer,
                         OUT buffer.bufferMemory);

    // Set the number of frames
    perFrameDescriptorSets = MHArray<MSArray<VkDescriptorSet, MAX_UNIFORM_SETS>>(framesCount);

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

    for (size_t i = 0; i < framesCount; i++) UpdateUniforms(bufferUpdates[i], imageUpdates[i]);

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

uint32_t Material::SetTexture(Hash::StringId id, const Texture2D& texture)
{
    using namespace Utils;
    using namespace Descriptor;

    auto texIndex = FindTextureIndex(texture.GetId());

    if (texIndex > -1) return texIndex;

    texIndex = textureIds.Count();
    textureIds.Append(texture.GetId());

    for (auto it = propertiesSlots.CreateIterator(); it; ++it)
    {
        auto& slot = *it;
        auto& properties = slot.properties;

        auto propIdx = FindPropertyIndex(id, slot);

        if (propIdx == -1) continue;

        auto info = texture.GetInfo();

        textureInfos[texIndex] = {info.sampler, info.imageInfo.view, info.imageInfo.layout};

        auto prop = properties[propIdx];

        for (auto write = imageUpdates.CreateFIterator(); write; ++write)
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
                textureInfos.Append({defaultTexture2DInfo.sampler,
                                     defaultTexture2DInfo.imageInfo.view,
                                     defaultTexture2DInfo.imageInfo.layout});
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
                           .WithDepthWriting(isWritingDepth)
                           .WithProperties(layouts)
                           .Build();
}

void Material::Update()
{
    auto currentFrameIdx = Renderer::GetCurrentFrameIndex();

    auto& targetBuffers = bufferUpdates[currentFrameIdx];
    auto& targetImages = imageUpdates[currentFrameIdx];

    using Vulkan::Context;

    //    Write(targetSets);
    UpdateUniforms(targetBuffers, targetImages);

    targetBuffers.Clear();
}

void Material::Update(Hash::StringId id)
{
    for (auto it = propertiesSlots.CreateFIterator(); it; ++it)
    {
        auto propertyIndex = FindPropertyIndex(id, *it);

        if (propertyIndex == -1) return;

        auto& buffersToWrite = bufferUpdates[Renderer::GetCurrentFrameIndex()];
        auto& imagesToWrite = imageUpdates[Renderer::GetCurrentFrameIndex()];
        //        Write(setsToWrite);
        UpdateUniforms(buffersToWrite, imagesToWrite);

        buffersToWrite.Clear();
        imagesToWrite.Clear();
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

        for (auto setIt = perFrameDescriptorSets.CreateFIterator(); setIt; ++setIt)
        {
            auto sets = *setIt;
            if (IsTexture2D(prop.type))
                QueueImageUpdate(imageUpdates[setIt.GetIndex()], sets[set], propIt.GetIndex());
            else
                QueuePropertyUpdate(bufferUpdates[setIt.GetIndex()],
                                    sets[set],
                                    prop.type,
                                    propIt.GetIndex(),
                                    1,
                                    {buffer.buffer, prop.offset, prop.size});
        }
    }
}

void Material::UpdateUniforms(MHArray<UniformBufferUpdate>& buffers,
                              MHArray<UniformImageUpdate>& images)
{
    using namespace Vulkan::Utils::Descriptor;

    MSArray<VkWriteDescriptorSet, 10> writeSets;
    MSArray<VkDescriptorBufferInfo, MAX_UNIFORM_SETS * 10> bufferInfs;
    MSArray<VkDescriptorImageInfo, MAX_TEXTURES> imageInfos;

    for (auto it = buffers.CreateIterator(); it; ++it)
    {
        auto& update = it->update;
        auto& bufferInfo = it->bufferUpdate;

        bufferInfs.Append({bufferInfo.buffer, bufferInfo.offset, bufferInfo.range});
        writeSets.Append(CreateWriteSet(update.dstBinding,
                                        update.set,
                                        update.descriptors,
                                        Utils::ToVkDescriptorType(update.type),
                                        &bufferInfs.Back()));
    }

    for (auto it = textureInfos.CreateIterator(); it; ++it)
    {
        imageInfos.Append({it->sampler, it->view, Utils::ToVkImageLayout(it->layout)});
    }

    for (auto it = images.CreateIterator(); it; ++it)
    {
        auto& update = it->update;

        writeSets.Append(WriteDescriptorImage(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                              update.set,
                                              imageInfos.Data(),
                                              update.dstBinding,
                                              update.descriptors,
                                              update.dstIndex));
    }

    Utils::Descriptor::WriteSets(Context::GetVkLogicalDevice(),
                                 writeSets.Data(),
                                 writeSets.Count());
    buffers.Clear();
    images.Clear();
}

void Material::QueueImageUpdate(MHArray<UniformImageUpdate>& imageUpdate,
                                VkDescriptorSet& set,
                                uint32_t binding,
                                uint32_t count,
                                uint32_t index)
{
    using namespace Vulkan::Utils::Descriptor;

    imageUpdate.Append({{set, binding, index, count, Utils::UniformType::TEXTURE2D}, textureInfos});
}

void Material::QueuePropertyUpdate(MHArray<UniformBufferUpdate>& bufferUpdateQueue,
                                   VkDescriptorSet& set,
                                   Utils::UniformType type,
                                   uint32_t binding,
                                   uint32_t count,
                                   BufferData bufferInfo)
{
    using namespace Vulkan::Utils::Descriptor;

    bufferUpdateQueue.Append({{set, binding, 0, count, type}, bufferInfo});
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
    auto tmpPushConstant = pushConstant;
    auto tmpIsWritingDepth = isWritingDepth;

    auto tmpTextureInfos = std::move(textureInfos);
    auto tmpImageUpdates = std::move(imageUpdates);
    auto tmpBufferUpdates = std::move(bufferUpdates);

    vertexShader = std::move(other.vertexShader);
    fragmentShader = std::move(other.fragmentShader);
    bufferSize = other.bufferSize;
    buffer = other.buffer;
    graphicsPipeline = std::move(other.graphicsPipeline);
    propertiesSlots = other.propertiesSlots;
    perFrameDescriptorSets = std::move(other.perFrameDescriptorSets);
    pushConstant = other.pushConstant;
    isWritingDepth = other.isWritingDepth;
    textureInfos = std::move(other.textureInfos);
    imageUpdates = std::move(other.imageUpdates);
    bufferUpdates = std::move(other.bufferUpdates);

    other.vertexShader = std::move(tmpVertexShader);
    other.fragmentShader = std::move(tmpFragmentShader);
    other.bufferSize = tmpBufferSize;
    other.buffer = tmpBuffer;
    other.graphicsPipeline = std::move(tmpGraphicsPipeline);
    other.propertiesSlots = tmpPropertiesSlots;
    other.perFrameDescriptorSets = std::move(tmpPerFrameDescriptors);
    other.pushConstant = tmpPushConstant;
    other.isWritingDepth = tmpIsWritingDepth;

    other.textureInfos = std::move(tmpTextureInfos);
    other.imageUpdates = std::move(tmpImageUpdates);
    other.bufferUpdates = std::move(tmpBufferUpdates);
}
} // namespace Siege::Vulkan
