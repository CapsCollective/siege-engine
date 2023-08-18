//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Shader.h"

#include <utils/Logging.h>

#include <fstream>

#include "render/renderer/renderer/Renderer3D.h"
#include "utils/ShaderModule.h"

namespace Siege::Vulkan
{
Shader::VertexBinding& Shader::VertexBinding::WithInputRate(Utils::VertexInputRate rate)
{
    inputRate = rate;
    return *this;
}

Shader::VertexBinding& Shader::VertexBinding::AddFloatVec3Attribute()
{
    attributes.Append({stride, Utils::VertexAttributeType::VERTEX_FLOAT_VEC3, inputRate});
    stride += sizeof(float) * 3;
    return *this;
}

Shader::VertexBinding& Shader::VertexBinding::AddFloatVec4Attribute()
{
    attributes.Append({stride, Utils::VertexAttributeType::VERTEX_FLOAT_VEC4, inputRate});
    stride += sizeof(float) * 4;
    return *this;
}

Shader::VertexBinding& Shader::VertexBinding::AddFloatVec2Attribute()
{
    attributes.Append({stride, Utils::VertexAttributeType::VERTEX_FLOAT_VEC2, inputRate});
    stride += sizeof(float) * 2;
    return *this;
}

Shader::VertexBinding& Shader::VertexBinding::AddMat4Attribute()
{
    return AddFloatVec4Attribute()
        .AddFloatVec4Attribute()
        .AddFloatVec4Attribute()
        .AddFloatVec4Attribute();
}

Shader::Builder& Shader::Builder::WithGlobalData3DUniform(uint32_t set)
{
    return WithUniform<Siege::Renderer3D::GlobalData>("globalData", set);
}

Shader::Builder& Shader::Builder::WithPushConstant(uint64_t size)
{
    pushConstant.size = size;
    return *this;
}

Shader::Builder& Shader::Builder::WithVertexTopology(Utils::PipelineTopology topology)
{
    expectedTopology = topology;
    return *this;
}

Shader::Builder& Shader::Builder::WithTexture(const String& name, uint32_t set, uint32_t count)
{
    uniforms.Append({INTERN_STR(name.Str()),
                     0,
                     set,
                     static_cast<uint32_t>(uniforms.Count()),
                     count,
                     Utils::TEXTURE2D});

    return *this;
}

Shader::Builder& Shader::Builder::WithDefaultTexture(const Texture2D* texture)
{
    defaultTextureInfo = texture->GetInfo();
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
                  defaultTextureInfo,
                  pushConstant,
                  totalUniformSize,
                  attributeCount);
}

Shader::Shader(const Shader& other) :
    filePath {other.filePath},
    type {other.type},
    expectedUniforms {other.expectedUniforms},
    vertexBindings {other.vertexBindings},
    defaultTextureInfo {other.defaultTextureInfo},
    pushConstant {other.pushConstant},
    totalUniformSize {other.totalUniformSize}
{
    CreateShaderModule();
}

Shader::Shader(const String& filePath,
               Utils::ShaderType type,
               Utils::PipelineTopology expectedTopology,
               MSArray<Uniform, 10> uniforms,
               MSArray<VertexBinding, 5> vertices,
               Texture2D::Info tex2DInfo,
               PushConstant pushConstant,
               size_t totalSize,
               uint32_t totalVertexAttributes) :
    filePath {filePath},
    type {type},
    expectedTopology {expectedTopology},
    expectedUniforms {uniforms},
    vertexBindings {vertices},
    defaultTextureInfo {tex2DInfo},
    pushConstant {pushConstant},
    totalUniformSize {totalSize},
    totalVertexAttributeCount {totalVertexAttributes}
{
    CreateShaderModule();
}

Shader::~Shader()
{
    Destroy();
}

void Shader::Destroy()
{
    Utils::Shader::DestroyShaderModule(shaderModule);
}

MHArray<char> Shader::ReadFileAsBinary(const String& filePath)
{
    // Read the file as binary and consume the entire file.
    std::ifstream file {filePath.Str(), std::ios::ate | std::ios::binary};

    CC_ASSERT(file.is_open(), String("Could not find file: ") + filePath)

    // Since we consumed the entire file, we can tell the size by checking where
    // the file stream is reading from (which presumably is at the end of the file).
    uint32_t size = static_cast<uint32_t>(file.tellg());

    MHArray<char> buffer(size);

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
    auto tmpDefaultTexture2DInfo = defaultTextureInfo;
    auto tmpPushConstant = pushConstant;

    filePath = other.filePath;
    type = other.type;
    shaderModule = other.shaderModule;
    expectedUniforms = other.expectedUniforms;
    vertexBindings = other.vertexBindings;
    totalUniformSize = other.totalUniformSize;
    totalVertexAttributeCount = other.totalVertexAttributeCount;
    expectedTopology = other.expectedTopology;
    defaultTextureInfo = other.defaultTextureInfo;
    pushConstant = other.pushConstant;

    other.filePath = tmpFilePath;
    other.type = tmpShaderType;
    other.shaderModule = tmpShaderModule;
    other.expectedUniforms = tmpExpectedUniforms;
    other.vertexBindings = tmpVertexBindings;
    other.totalUniformSize = tmpTotalUniformSize;
    other.totalVertexAttributeCount = tmpTotalAttributeCount;
    other.expectedTopology = tmpExpectedTopology;
    other.defaultTextureInfo = tmpDefaultTexture2DInfo;
    other.pushConstant = tmpPushConstant;
}
} // namespace Siege::Vulkan
