//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_MATERIAL_H
#define SIEGE_ENGINE_VULKAN_MATERIAL_H

#include "CommandBuffer.h"
#include "Pipeline.h"
#include "Shader.h"

namespace Siege::Vulkan
{
class Material
{
public:
    Material() = default;
    Material(Shader  vertShader, Shader  fragShader);
    Material(Material&& other) noexcept ;

    ~Material();

    Material& operator=(Material&& other);

    void SetUniformData(Hash::StringId id, uint64_t dataSize, const void* data);

    void Bind(const CommandBuffer& commandBuffer);
    // TODO: Remove the next function once we incorporate the CommandBuffer class into renderer
    void Bind(VkCommandBuffer commandBuffer);
private:

    struct Property
    {
        Hash::StringId id {0};
        uint32_t binding {0};
        uint64_t offset {0};
        uint64_t size {0};
        Utils::UniformType type {Utils::UNKNOWN};
        Utils::ShaderType shaderStage {Utils::EMPTY};
    };

    struct PropertiesSlot
    {
        VkDescriptorSet set {nullptr};
        VkDescriptorSetLayout layout {VK_NULL_HANDLE};
        ::Siege::Utils::MSArray<Property, 10> properties;
    };

    void Swap(Material& other);
    void AddShader(const Shader& shader, uint64_t& offset);

    void UpdateMaterial();
    void WriteSet(PropertiesSlot& slot);

    Pipeline graphicsPipeline;

    Shader vertexShader;
    Shader fragmentShader;

    uint64_t bufferSize {0};
    Buffer::Buffer buffer;

    ::Siege::Utils::MSArray<PropertiesSlot, 10> propertiesSlots;
};
} // namespace Siege

#endif // SIEGE_ENGINE_VULKAN_MATERIAL_H
