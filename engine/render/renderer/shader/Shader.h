//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SHADER_H
#define SIEGE_ENGINE_SHADER_H

#include <utils/Hash.h>

#include <map>

#include "../Core.h"
#include "../buffer/Buffer.h"
#include "../pipeline/Pipeline.h"

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
        Hash::StringId id;
        uint32_t binding = 0;
        uint64_t size = 0;
        size_t arraySize = 0;
        size_t dynamicCount = 1;
        VkDescriptorType type;
    };

    struct VertexBinding
    {
        static constexpr size_t MAX_VERTEX_ATTRIBUTES = 10;
        StackArray<VertexDescription::Attribute, MAX_VERTEX_ATTRIBUTES> attributes;
        uint32_t vertexStride = 0;
    };

    Shader();
    ~Shader();

    static Shader BuildShader();

    Shader& FromShader(const String& filePath);
    Shader& WithStage(PipelineConfig::PipelineStage stage);

    Shader& WithUniform(uint32_t binding, const String& name, uint64_t size, size_t arraySize = 1);
    Shader& WithDynamicUniform(uint32_t binding,
                               const String& name,
                               uint64_t size,
                               size_t arraySize,
                               size_t count);
    Shader& WithStorage(uint32_t binding, const String& name, uint64_t size, size_t arraySize = 1);
    Shader& WithDynamicStorage(uint32_t binding,
                               const String& name,
                               uint64_t size,
                               size_t arraySize,
                               size_t count);

    Shader& WithVertexType(uint32_t size);
    Shader& WithVertexAttribute(uint32_t offset, VertexDescription::AttributeType type);

    const StackArray<VertexBinding, MAX_UNIFORMS>& GetVertexBindings() const
    {
        return vertexBindings;
    }
    const StackArray<Uniform, MAX_UNIFORMS>& GetUniforms() const
    {
        return uniforms;
    }
    StackArray<VertexBinding, MAX_UNIFORMS>& GetVertexBindings()
    {
        return vertexBindings;
    }
    StackArray<Uniform, MAX_UNIFORMS>& GetUniforms()
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

    uint64_t GetUniformSize()
    {
        return sizeOfUniforms;
    }

    void SetUniformType(uint32_t binding,
                        const String& name,
                        VkDescriptorType type,
                        uint64_t size,
                        size_t arraySize,
                        size_t count = 1);

    String filePath;

    StackArray<Uniform, MAX_UNIFORMS> uniforms;
    StackArray<VertexBinding, MAX_UNIFORMS> vertexBindings;

    PipelineConfig::PipelineStage stage;
    uint64_t sizeOfUniforms = 0;
};
} // namespace Siege

#endif
