//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_SHADER_H
#define SIEGE_ENGINE_VULKAN_SHADER_H

#include <utils/String.h>

#include "render/renderer/buffer/Buffer.h"
#include "utils/Types.h"

#include <utils/collections/StackArray.h>
#include <utils/collections/HeapArray.h>

namespace Siege::Vulkan
{
class Shader
{
public:
    struct Uniform
    {
        Hash::StringId id {0};
        uint64_t allocatedSize {0};
        uint32_t dynamicSize {0};
        uint32_t totalSize {0};
        uint32_t set {0};
        uint32_t slot {0};
        Utils::UniformType type {Utils::UNKNOWN};
    };

    struct VertexAttribute
    {
        uint64_t offset {0};
        Utils::VertexAttributeType type {Utils::VertexAttributeType::VERTEX_UNDEFINED};
    };

    struct VertexBinding
    {
    public:
        VertexBinding& AddFloatVec3Attribute();

        VertexBinding& AddFloatVec4Attribute();

        VertexBinding& AddFloatVec2Attribute();

        static constexpr uint64_t MAX_VERTEX_ATTRIBUTES {10};
        ::Siege::Utils::MSArray<VertexAttribute, MAX_VERTEX_ATTRIBUTES> attributes;
        uint64_t stride {0};
    };

    struct Builder
    {
        Builder& FromVertexShader(const String& path);

        Builder& FromFragmentShader(const String& path);

        Builder& WithVertexBinding(const VertexBinding& binding);

        template<typename T>
        inline Builder& WithUniform(const String& name, uint32_t set = 0, uint64_t size = 1, uint32_t count = 1)
        {
            auto paddedSize = Buffer::PadUniformBufferSize(sizeof(T));

            uniforms.Append({INTERN_STR(name.Str()),
                             paddedSize,
                             count,
                             static_cast<uint32_t>(uniforms.Count()),
                             Utils::UNIFORM});

            totalUniformSize += (paddedSize * size) * count;

            return *this;
        }

        template<typename T>
        inline Builder& WithStorage(const String& name, uint32_t set = 0,  uint64_t size = 1, uint32_t count = 1)
        {
            auto paddedSize = Buffer::PadUniformBufferSize(sizeof(T));

            uniforms.Append({INTERN_STR(name.Str()),
                             paddedSize,
                             count,
                             static_cast<uint32_t>(uniforms.Count()),
                             Utils::STORAGE});

            totalUniformSize += (paddedSize * size) * count;

            return *this;
        }

        Builder& WithGlobalData3DUniform(uint32_t set = 0);

        Builder& WithGlobalData2DUniform(uint32_t set = 0);

        Builder& WithTransform3DStorage(uint32_t set = 0, uint64_t size = 1);

        Builder& WithTransform2DStorage(uint32_t set = 0, uint64_t size = 1);

        Builder& WithTexture(const String& name, uint32_t set = 0, uint32_t count = 1);

        Shader Build() const;

        String filePath {""};
        Utils::ShaderType type {Utils::ALL_GRAPHICS};
        ::Siege::Utils::MSArray<Uniform, 10> uniforms;
        ::Siege::Utils::MSArray<VertexBinding, 5> vertexBindings;
        uint64_t totalUniformSize {0};
    };

    Shader() = default;

    Shader(const Shader& other);

    inline Shader(Shader&& other) { Swap(other); }

    ~Shader();

    static ::Siege::Utils::MHArray<char> ReadFileAsBinary(const String& filePath);

    inline Shader& operator=(Shader&& other) noexcept
    {
        Swap(other);
        return *this;
    }

    inline Shader& operator=(const Shader& other) noexcept
    {
        filePath = other.filePath;

        CreateShaderModule();

        type = other.type;
        expectedUniforms = other.expectedUniforms;
        vertexBindings = other.vertexBindings;
        totalUniformSize = other.totalUniformSize;

        return *this;
    }

    const size_t& GetTotalUniformSize() { return totalUniformSize; }

    const ::Siege::Utils::MSArray<VertexBinding, 5>& GetVertexBindings() const { return vertexBindings; }
    inline const ::Siege::Utils::MSArray<Uniform, 10>& GetUniforms() const { return expectedUniforms; }

    inline Utils::ShaderType GetShaderType() const { return type; }

private:
    void Swap(Shader& other);
    void CreateShaderModule();

    String filePath {""};
    Utils::ShaderType type {Utils::ShaderType::EMPTY};
    VkShaderModule shaderModule {nullptr};
    ::Siege::Utils::MSArray<Uniform, 10> expectedUniforms;
    ::Siege::Utils::MSArray<VertexBinding, 5> vertexBindings;
    size_t totalUniformSize {0};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_SHADER_H
