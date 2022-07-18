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

Shader& Shader::WithUniform(u32 binding, const String& name, u64 size, size_t arraySize)
{
    SetUniformType(binding, name, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, size, arraySize);

    std::cout << "Added uniform " << name << " for binding: " << binding << std::endl;

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

    std::cout << "Added dynamic uniform " << name << " for binding: " << binding << std::endl;

    return *this;
}

Shader& Shader::WithStorage(u32 binding, const String& name, u64 size, size_t arraySize)
{
    SetUniformType(binding, name, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, size, arraySize);

    std::cout << "Added storage " << name << "for binding: " << binding << std::endl;

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

    std::cout << "Added dynamic storage " << name << " for binding: " << binding << std::endl;

    return *this;
}

Shader& Shader::WithVertexType(u32 size)
{
    size_t index = vertexBindings.Count();

    vertexBindings.Activate(index);
    auto& binding = vertexBindings.Get(index);
    binding.vertexStride = size;

    std::cout << "Added new vertex type of size " << size << std::endl;
    std::cout << "There are now " << vertexBindings.Count() << " bindings" << std::endl;

    return *this;
}

Shader& Shader::WithVertexAttribute(u32 offset, VertexDescription::AttributeType type)
{
    size_t index = vertexBindings.Count() - 1;

    CC_ASSERT(index >= 0, "A vertex type must be added before creating attributes!");

    auto& binding = vertexBindings.Get(index);
    auto& attributes = binding.attributes;

    attributes.Append({offset, type});

    std::cout << "Added new vertex attribute for binding " << index << std::endl;
    std::cout << "Binding now has " << binding.attributes.Count() << " attributes" << std::endl;

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
              String("ERROR: Maximum number of uniforms have been reached. Maximum is " +
                          String(uniforms.Size())).Str());

    StringId strId = INTERN_STR(name);

    auto paddedSize = Buffer::PadUniformBufferSize(size);

    uniforms.Append({strId, binding, paddedSize, arraySize, count, type});

    sizeOfUniforms += (paddedSize * arraySize) * count;
}
} // namespace Siege