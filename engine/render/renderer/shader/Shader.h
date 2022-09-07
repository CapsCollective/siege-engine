//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SHADER_H
#define SIEGE_ENGINE_SHADER_H

#include <map>

#include "../Core.h"
#include "../buffer/Buffer.h"
#include "../pipeline/Pipeline.h"
#include "../utils/Hash.h"

namespace Siege
{
class Shader
{
public:

    static constexpr size_t MAX_UNIFORMS = 5;

    enum DescriptorType
    {
        UNIFORM = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        UNIFORM_DYNAMIC = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        STORAGE = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        STORAGE_DYNAMIC = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
    };

    enum ShaderStage
    {
        VERTEX = VK_SHADER_STAGE_VERTEX_BIT,
        FRAGMENT = VK_SHADER_STAGE_FRAGMENT_BIT,
        ALL_GRAPHICS = VK_SHADER_STAGE_ALL_GRAPHICS,
    };

    struct Uniform
    {
        Utils::StringId id;
        u32 binding = 0;
        u64 size = 0;
        size_t arraySize = 0;
        size_t dynamicCount = 1;
        VkDescriptorType type;
    };

    struct VertexBinding
    {
        static constexpr size_t MAX_VERTEX_ATTRIBUTES = 10;
        Utils::StackArray<VertexDescription::Attribute, MAX_VERTEX_ATTRIBUTES> attributes;
        u32 vertexStride = 0;
    };

    Shader();
    ~Shader();

    static Shader BuildShader();

    Shader& FromShader(const String& filePath);
    Shader& WithStage(PipelineConfig::PipelineStage stage);

    Shader& WithUniform(u32 binding, const String& name, u64 size, size_t arraySize = 1);
    Shader& WithDynamicUniform(u32 binding,
                               const String& name,
                               u64 size,
                               size_t arraySize,
                               size_t count);
    Shader& WithStorage(u32 binding, const String& name, u64 size, size_t arraySize = 1);
    Shader& WithDynamicStorage(u32 binding,
                               const String& name,
                               u64 size,
                               size_t arraySize,
                               size_t count);

    Shader& WithVertexType(u32 size);
    Shader& WithVertexAttribute(u32 offset, VertexDescription::AttributeType type);

    const Utils::StackArray<VertexBinding, MAX_UNIFORMS>& GetVertexBindings() const
    {
        return vertexBindings;
    }
    const Utils::StackArray<Uniform, MAX_UNIFORMS>& GetUniforms() const
    {
        return uniforms;
    }
    Utils::StackArray<VertexBinding, MAX_UNIFORMS>& GetVertexBindings()
    {
        return vertexBindings;
    }
    Utils::StackArray<Uniform, MAX_UNIFORMS>& GetUniforms()
    {
        return uniforms;
    }

    const String& GetPath()
    {
        return filePath;
    }
    PipelineConfig::PipelineStage GetStage()
    {
        return stage;
    }

    u64 GetUniformSize()
    {
        return sizeOfUniforms;
    }

    void SetUniformType(u32 binding,
                        const String& name,
                        VkDescriptorType type,
                        u64 size,
                        size_t arraySize,
                        size_t count = 1);

    String filePath;

    Utils::StackArray<Uniform, MAX_UNIFORMS> uniforms;
    Utils::StackArray<VertexBinding, MAX_UNIFORMS> vertexBindings;

    PipelineConfig::PipelineStage stage;
    u64 sizeOfUniforms = 0;
};
} // namespace Siege

#endif