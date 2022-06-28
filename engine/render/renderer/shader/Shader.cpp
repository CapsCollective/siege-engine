//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Shader.h"
#include <utils/Logging.h>

namespace Siege
{
Shader::Shader() = default;

Shader::~Shader() = default;

Shader Shader::BuildShader()
{
    return {};
}

Shader& Shader::FromShader(const String& newFilePath)
{
    filePath = newFilePath;

    return *this;
}

Shader& Shader::WithStage(PipelineConfig::PipelineStage newStage)
{
    stage = newStage;
    return *this;
}

Shader& Shader::WithUniform(u32 binding, const String& name, u64 size, size_t arraySize)
{
    SetUniformType(binding, name, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, size, arraySize);

    CC_LOG_INFO("SHADER: [{}] - Added uniform [{}] for binding [{}]",filePath, name, binding)

    return *this;
}

Shader& Shader::WithDynamicUniform(u32 binding,
                                   const String& name,
                                   u64 size,
                                   size_t arraySize,
                                   size_t count)
{
    SetUniformType(binding,
                   name,
                   VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                   size,
                   arraySize,
                   count);

    CC_LOG_INFO("SHADER: [{}] - Added dynamic uniform [{}] for binding [{}]",filePath, name, binding)

    return *this;
}

Shader& Shader::WithStorage(u32 binding, const String& name, u64 size, size_t arraySize)
{
    SetUniformType(binding, name, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, size, arraySize);

    CC_LOG_INFO("SHADER: [{}] - Added storage uniform [{}] for binding [{}]",filePath, name, binding)

    return *this;
}

Shader& Shader::WithDynamicStorage(u32 binding,
                                   const String& name,
                                   u64 size,
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

Shader& Shader::WithVertexType(u32 size)
{
    size_t index = vertexBindings.Count();

    vertexBindings.Activate(index);
    auto& binding = vertexBindings.Get(index);
    binding.vertexStride = size;

    CC_LOG_INFO("SHADER: [{}] - Vertex [{}/{}] added with size: {}",
                filePath,
                static_cast<uint64_t>(vertexBindings.Count()),
                static_cast<uint64_t>(MAX_UNIFORMS),
                size)

    return *this;
}

Shader& Shader::WithVertexAttribute(u32 offset, VertexDescription::AttributeType type)
{
    size_t index = vertexBindings.Count() - 1;

    CC_ASSERT(index >= 0, "A vertex type must be added before creating attributes!")

    auto& binding = vertexBindings.Get(index);
    auto& attributes = binding.attributes;

    attributes.Append({offset, type});

    CC_LOG_INFO("SHADER: [{}] - Vertex attribute [{}/{}] added for binding [{}]",
                filePath,
                static_cast<uint64_t>(binding.attributes.Count()),
                static_cast<uint64_t>(binding.MAX_VERTEX_ATTRIBUTES),
                static_cast<uint64_t>(index))

    return *this;
}

void Shader::SetUniformType(u32 binding,
                            const String& name,
                            VkDescriptorType type,
                            u64 size,
                            size_t arraySize,
                            size_t count)
{
    CC_ASSERT(uniforms.Count() <= uniforms.Size(),
              std::string("ERROR: Maximum number of uniforms have been reached. Maximum is " +
                          std::to_string(uniforms.Size()))
                  .c_str())

    Utils::StringId strId = INTERN_STR(name);

    auto paddedSize = Buffer::PadUniformBufferSize(size);

    uniforms.Append({strId, binding, paddedSize, arraySize, count, type});

    sizeOfUniforms += (paddedSize * arraySize) * count;
}
} // namespace Siege