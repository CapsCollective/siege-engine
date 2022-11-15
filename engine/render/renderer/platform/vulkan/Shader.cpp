//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Shader.h"

#include <fstream>

#include "render/renderer/model/Model.h"
#include "render/renderer/renderer/Renderer3D.h"
#include "render/renderer/renderer/Renderer2D.h"
#include "utils/TypeAdaptor.h"

#include <utils/Logging.h>

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
    auto paddedSize = Buffer::PadUniformBufferSize(sizeof(Siege::Renderer3D::GlobalData));

    uniforms.Append({INTERN_STR("globalData"),
                     paddedSize,
                     1,
                     static_cast<uint32_t>(paddedSize),
                     set,
                     static_cast<uint32_t>(uniforms.Count()),
                     Utils::UNIFORM});

    totalUniformSize += paddedSize;

    return *this;
}

Shader::Builder& Shader::Builder::WithGlobalData2DUniform(uint32_t set)
{
    auto paddedSize = Buffer::PadUniformBufferSize(sizeof(Siege::Renderer2D::GlobalData));

    uniforms.Append({INTERN_STR("globalData"),
                     paddedSize,
                     1,
                     static_cast<uint32_t>(paddedSize),
                     set,
                     static_cast<uint32_t>(uniforms.Count()),
                     Utils::UNIFORM});

    totalUniformSize += paddedSize;

    return *this;
}

Shader::Builder& Shader::Builder::WithTransform2DStorage(uint32_t set, uint64_t size)
{
    auto paddedSize = Buffer::PadStorageBufferSize(sizeof(Siege::Model::Transform2D));

    uniforms.Append({INTERN_STR("transforms"),
                     paddedSize,
                     1,
                     static_cast<uint32_t>(paddedSize * size),
                     set,
                     static_cast<uint32_t>(uniforms.Count()),
                     Utils::STORAGE});

    totalUniformSize += (paddedSize * size);
    return *this;
}

Shader::Builder& Shader::Builder::WithTransform3DStorage(uint32_t set, uint64_t size)
{
    auto paddedSize = Buffer::PadStorageBufferSize(sizeof(Siege::Model::Transform));

    uniforms.Append({INTERN_STR("transforms"),
                     paddedSize,
                     1,
                     static_cast<uint32_t>(paddedSize * size),
                     set,
                     static_cast<uint32_t>(uniforms.Count()),
                     Utils::STORAGE});

    totalUniformSize += (paddedSize * size);
    return *this;
}

Shader::Builder& Shader::Builder::WithTexture(const String& name, uint32_t set, uint32_t count)
{
    uniforms.Append({INTERN_STR(name.Str()),
                     0,
                     count,
                     set,
                     static_cast<uint32_t>(uniforms.Count()),
                     Utils::TEXTURE});
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
    return *this;
}

Shader Shader::Builder::Build() const
{
    Shader shader;

    shader.filePath = filePath;
    shader.type = type;
    shader.expectedUniforms = uniforms;
    shader.vertexBindings = vertexBindings;
    shader.totalUniformSize = totalUniformSize;

    for (size_t i = 0; i < vertexBindings.Count(); i++)
    {
        shader.totalVertexAttributeCount += vertexBindings[i].attributes.Count();
    }

    shader.CreateShaderModule();

    return shader;
}

Shader::Shader(const Shader& other)
    : filePath{other.filePath},
    type{other.type},
    expectedUniforms{other.expectedUniforms},
    vertexBindings{other.vertexBindings},
    totalUniformSize{other.totalUniformSize}
{
    CreateShaderModule();
}

Shader::~Shader()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    vkDestroyShaderModule(device, shaderModule, nullptr);
}

::Siege::Utils::MHArray<char> Shader::ReadFileAsBinary(const String& filePath)
{
    // Read the file as binary and consume the entire file.
    std::ifstream file {filePath.Str(), std::ios::ate | std::ios::binary};

    CC_ASSERT(file.is_open(), String("Could not find file: ") + filePath);

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

    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.Size();

    // Because the code is expected to be numerical, we need to cast the values in the
    // array to 32-bit unsigned integers.
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.Data());

    CC_ASSERT(vkCreateShaderModule(Vulkan::Context::GetVkLogicalDevice(),
                                   &createInfo,
                                   nullptr,
                                   OUT &shaderModule) == VK_SUCCESS,
              "Failed to create shader module!")
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

    filePath = other.filePath;
    type = other.type;
    shaderModule = other.shaderModule;
    expectedUniforms = other.expectedUniforms;
    vertexBindings = other.vertexBindings;
    totalUniformSize = other.totalUniformSize;
    totalVertexAttributeCount = other.totalVertexAttributeCount;

    other.filePath = tmpFilePath;
    other.type = tmpShaderType;
    other.shaderModule = tmpShaderModule;
    other.expectedUniforms = tmpExpectedUniforms;
    other.vertexBindings = tmpVertexBindings;
    other.totalUniformSize = tmpTotalUniformSize;
    other.totalVertexAttributeCount = tmpTotalAttributeCount;
}
} // namespace Siege::Vulkan