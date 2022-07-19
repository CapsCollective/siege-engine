//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Shader.h"

namespace Siege
{
Shader::Shader() {}

Shader::~Shader() {}

Shader Shader::BuildShader()
{
    return Shader();
}

Shader& Shader::FromShader(const String& filePath)
{
    this->filePath = filePath;

    return *this;
}

Shader& Shader::WithStage(PipelineConfig::PipelineStage stage)
{
    this->stage = stage;
    return *this;
}

Shader& Shader::WithUniform(uint32_t binding, const String& name, uint64_t size, size_t arraySize)
{
    SetUniformType(binding, name, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, size, arraySize);

    CC_LOG_INFO("SHADER: [{}] - Added uniform [{}] for binding [{}]", filePath, name, binding)

    return *this;
}

Shader& Shader::WithDynamicUniform(uint32_t binding,
                                   const String& name,
                                   uint64_t size,
                                   size_t arraySize,
                                   size_t count)
{
    SetUniformType(binding,
                   name,
                   VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                   size,
                   arraySize,
                   count);

    CC_LOG_INFO("SHADER: [{}] - Added dynamic uniform [{}] for binding [{}]",
                filePath,
                name,
                binding)

    return *this;
}

Shader& Shader::WithStorage(uint32_t binding, const String& name, uint64_t size, size_t arraySize)
{
    SetUniformType(binding, name, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, size, arraySize);

    CC_LOG_INFO("SHADER: [{}] - Added storage uniform [{}] for binding [{}]",
                filePath,
                name,
                binding)

    return *this;
}

Shader& Shader::WithDynamicStorage(uint32_t binding,
                                   const String& name,
                                   uint64_t size,
                                   size_t arraySize,
                                   size_t count)
{
    SetUniformType(binding,
                   name,
                   VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
                   size,
                   arraySize,
                   count);

    CC_LOG_INFO("SHADER: [{}] - Added dynamic storage uniform [{}] for binding [{}]",
                filePath,
                name,
                binding)

    return *this;
}

Shader& Shader::WithVertexType(uint32_t size)
{
    size_t index = vertexBindings.Count();

    vertexBindings.Activate(index);
    auto& binding = vertexBindings.Get(index);
    binding.vertexStride = size;

    CC_LOG_INFO("SHADER: [{}] - Vertex [{}/{}] added with size: {}",
                filePath,
                vertexBindings.Count(),
                MAX_UNIFORMS,
                size)

    return *this;
}

Shader& Shader::WithVertexAttribute(uint32_t offset, VertexDescription::AttributeType type)
{
    size_t index = vertexBindings.Count() - 1;

    CC_ASSERT(index >= 0, "A vertex type must be added before creating attributes!");

    auto& binding = vertexBindings.Get(index);
    auto& attributes = binding.attributes;

    attributes.Append({offset, type});

    CC_LOG_INFO("SHADER: [{}] - Vertex attribute [{}/{}] added for binding [{}]",
                filePath,
                binding.attributes.Count(),
                binding.MAX_VERTEX_ATTRIBUTES,
                index)

    return *this;
}

void Shader::SetUniformType(uint32_t binding,
                            const String& name,
                            VkDescriptorType type,
                            uint64_t size,
                            size_t arraySize,
                            size_t count)
{
    CC_ASSERT(uniforms.Count() <= uniforms.Size(),
              String("ERROR: Maximum number of uniforms have been reached. Maximum is " +
                     String(uniforms.Size()))
                  .Str());

    Hash::StringId strId = INTERN_STR(name);

    auto paddedSize = Buffer::PadUniformBufferSize(size);

    uniforms.Append({strId, binding, paddedSize, arraySize, count, type});

    sizeOfUniforms += (paddedSize * arraySize) * count;
}
} // namespace Siege