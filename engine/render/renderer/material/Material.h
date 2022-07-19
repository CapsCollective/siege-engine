//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MATERIAL_H
#define SIEGE_ENGINE_MATERIAL_H

#include "../Core.h"
#include "../buffer/Buffer.h"
#include "../descriptor/DescriptorPool.h"
#include "../pipeline/Pipeline.h"
#include "../shader/Shader.h"

namespace Siege
{
class Material
{
public:

    static constexpr size_t MAX_SHADERS = 2;
    static constexpr size_t MAX_BINDINGS_PER_SHADER = 5;
    static constexpr size_t MAX_MATERIAL_BINDINGS = MAX_SHADERS * MAX_BINDINGS_PER_SHADER;

    enum PolygonMode
    {
        FILL = 0,
        LINE = 1,
        POINT = 2
    };

    enum Topology
    {
        LINE_LIST = 1,
        LINE_STRIP = 2,
        TRIANGLE_LIST = 3
    };

    Material();
    Material(Shader* vertexShader);
    Material(Shader* vertexShader, Shader* fragmentShader);

    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;

    ~Material();

    void CreateDescriptors();

    void SetPolygonMode(PolygonMode mode)
    {
        shaderSettings.mode = mode;
    }
    void SetTopology(Topology topology)
    {
        shaderSettings.topology = topology;
    }
    void BuildMaterial();

    void SetUniformData(VkDeviceSize dataSize, const void* data);
    void SetUniformData(Hash::StringId id, VkDeviceSize dataSize, const void* data);
    void SetUniformData(const String& name, VkDeviceSize dataSize, const void* data);

    void SetVertexShader(Shader* shader);
    void SetFragmentShader(Shader* shader);

    bool HasProperty(Hash::StringId id);

    void Bind(VkCommandBuffer commandBuffer);
    void CreatePipeline();
    void RecreatePipeline();

    void DestroyMaterial();

    static void BuildMaterials(std::initializer_list<Material*> materials);

private:

    struct DescriptorBinding
    {
        VkDescriptorSetLayout layout {VK_NULL_HANDLE};
        VkDescriptorSet descriptorSet {VK_NULL_HANDLE};
        Shader::DescriptorType type;
    };

    struct Property
    {
        uint32_t binding = 0;
        Hash::StringId id = 0;
        VkShaderStageFlags stage;
        uint64_t offset = 0;
        uint64_t size = 0;
        size_t count = 0;
        DescriptorBinding descriptorBinding;
    };

    Material(Shader* vertexShader, Shader* fragmentShader, uint32_t shaderCount);

    Property& GetProperty(Hash::StringId id);
    void AddShader(Shader* shader);
    void SetShaderProperties(Shader* shader, uint64_t& offset);

    void CreateLayout(VkDescriptorSetLayout* layouts = nullptr,
                      uint32_t layoutCount = 0,
                      VkPushConstantRange* pushConstants = nullptr,
                      uint32_t pushConstantCount = 0);

    struct
    {
        PolygonMode mode = PolygonMode::FILL;
        Topology topology = Topology::TRIANGLE_LIST;
    } shaderSettings;

    size_t vertexCount = 0;
    uint32_t shaderCount = 0;

    Shader* vertexShader {nullptr};
    Shader* fragmentShader {nullptr};

    StackArray<Property, MAX_MATERIAL_BINDINGS> propertiesArray;

    Buffer::Buffer buffer;
    uint64_t bufferSize = 0;

    StackArray<VkDescriptorSet, MAX_MATERIAL_BINDINGS> descriptorSets;
    StackArray<uint32_t, MAX_MATERIAL_BINDINGS> descriptorOffsets;

    StackArray<VertexDescription::Binding, MAX_MATERIAL_BINDINGS> vertexBindings;

    Pipeline pipeline;
    VkPipelineLayout pipelineLayout {VK_NULL_HANDLE};

    bool isFreed = false;
};
} // namespace Siege

#endif
