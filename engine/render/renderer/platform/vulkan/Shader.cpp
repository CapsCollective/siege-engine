//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Shader.h"

#include <utils/Logging.h>

#include <fstream>

#include "render/renderer/model/Model.h"
#include "render/renderer/renderer/Renderer2D.h"
#include "render/renderer/renderer/Renderer3D.h"
#include "utils/ShaderModule.h"

namespace Siege::Vulkan
{
Shader::VertexBinding& Shader::VertexBinding::AddFloatVec3Attribute()
{
    attributes.Append({stride, Utils::VertexAttributeType::VERTEX_FLOAT_VEC3});
    stride += sizeof(float) * 3;
    return *this;
}

Shader::VertexBinding& Shader::VertexBinding::AddFloatVec4Attribute()
{
    attributes.Append({stride, Utils::VertexAttributeType::VERTEX_FLOAT_VEC4});
    stride += sizeof(float) * 4;
    return *this;
}

Shader::VertexBinding& Shader::VertexBinding::AddFloatVec2Attribute()
{
    attributes.Append({stride, Utils::VertexAttributeType::VERTEX_FLOAT_VEC2});
    stride += sizeof(float) * 2;
    return *this;
}

Shader::Builder& Shader::Builder::WithGlobalData3DUniform(uint32_t set)
{
    return WithUniform<Siege::Renderer3D::GlobalData>("globalData", set);
}

Shader::Builder& Shader::Builder::WithGlobalData2DUniform(uint32_t set)
{
    return WithUniform<Siege::Renderer2D::GlobalData>("globalData", set);
}

Shader::Builder& Shader::Builder::WithTransform2DStorage(uint32_t set, uint64_t size)
{
    return WithStorage<Siege::Model::Transform2D>("transforms", set, size);
}

Shader::Builder& Shader::Builder::WithTransform3DStorage(uint32_t set, uint64_t size)
{
    return WithStorage<Siege::Model::Transform>("transforms", set, size);
}

Shader::Builder& Shader::Builder::WithVertexTopology(Utils::PipelineTopology topology)
{
    expectedTopology = topology;
    return *this;
}

Shader::Builder& Shader::Builder::FromVertexShader(const String& path)
{
    filePath = path;
    type = Utils::VERTEX;

    return *this;
}

Shader::Builder& Shader::Builder::FromFragmentShader(const String& path)
{
    filePath = path;
    type = Utils::FRAGMENT;

    return *this;
}

Shader::Builder& Shader::Builder::WithVertexBinding(const Shader::VertexBinding& binding)
{
    vertexBindings.Append(binding);
    attributeCount += binding.attributes.Count();
    return *this;
}

Shader Shader::Builder::Build() const
{
    return Shader(filePath,
                  type,
                  expectedTopology,
                  uniforms,
                  vertexBindings,
                  totalUniformSize,
                  attributeCount);
}

Shader::Shader(const Shader& other) :
    filePath {other.filePath},
    type {other.type},
    expectedUniforms {other.expectedUniforms},
    vertexBindings {other.vertexBindings},
    totalUniformSize {other.totalUniformSize}
{
    CreateShaderModule();
}

Shader::Shader(const String& filePath,
               Utils::ShaderType type,
               Utils::PipelineTopology expectedTopology,
               ::Siege::Utils::MSArray<Uniform, 10> uniforms,
               ::Siege::Utils::MSArray<VertexBinding, 5> vertices,
               size_t totalSize,
               uint32_t totalVertexAttributes) :
    filePath {filePath},
    type {type},
    expectedTopology {expectedTopology},
    expectedUniforms {uniforms},
    vertexBindings {vertices},
    totalUniformSize {totalSize},
    totalVertexAttributeCount {totalVertexAttributes}
{
    CreateShaderModule();
}

Shader::~Shader()
{
    Utils::Shader::DestroyShaderModule(shaderModule);
}

::Siege::Utils::MHArray<char> Shader::ReadFileAsBinary(const String& filePath)
{
    // Read the file as binary and consume the entire file.
    std::ifstream file {filePath.Str(), std::ios::ate | std::ios::binary};

    CC_ASSERT(file.is_open(), String("Could not find file: ") + filePath)

    // Since we consumed the entire file, we can tell the size by checking where
    // the file stream is reading from (which presumably is at the end of the file).
    uint32_t size = static_cast<uint32_t>(file.tellg());

    ::Siege::Utils::MHArray<char> buffer(size);

    // Move to the beginning of the file.
    file.seekg(0);

    file.read(buffer.Data(), size);

    file.close();

    return buffer;
}

void Shader::CreateShaderModule()
{
    if (filePath == "") return;

    auto buffer = ReadFileAsBinary(filePath);

    shaderModule = Utils::Shader::CreateShaderModule(buffer);
}

void Shader::Swap(Shader& other)
{
    auto tmpFilePath = filePath;
    auto tmpShaderType = type;
    auto tmpShaderModule = shaderModule;
    auto tmpExpectedUniforms = expectedUniforms;
    auto tmpVertexBindings = vertexBindings;
    auto tmpTotalUniformSize = totalUniformSize;
    auto tmpTotalAttributeCount = totalVertexAttributeCount;
    auto tmpExpectedTopology = expectedTopology;

    filePath = other.filePath;
    type = other.type;
    shaderModule = other.shaderModule;
    expectedUniforms = other.expectedUniforms;
    vertexBindings = other.vertexBindings;
    totalUniformSize = other.totalUniformSize;
    totalVertexAttributeCount = other.totalVertexAttributeCount;
    expectedTopology = other.expectedTopology;

    other.filePath = tmpFilePath;
    other.type = tmpShaderType;
    other.shaderModule = tmpShaderModule;
    other.expectedUniforms = tmpExpectedUniforms;
    other.vertexBindings = tmpVertexBindings;
    other.totalUniformSize = tmpTotalUniformSize;
    other.totalVertexAttributeCount = tmpTotalAttributeCount;
    other.expectedTopology = tmpExpectedTopology;
}
} // namespace Siege::Vulkan
