#pragma once

#include "../Core.h"
#include "../Pipeline/Pipeline.h"
#include "../Buffer/Buffer.h"
#include "../Utils/Hash.h"
#include <map>

namespace SnekVk
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
        };

        struct Uniform
        {
            Utils::StringId id;
            u32 binding = 0; 
            u64 size;
            size_t count = 1;
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

        Shader& FromShader(const char* filePath);
        Shader& WithStage(PipelineConfig::PipelineStage stage);
        
        Shader& WithUniform(u32 binding, const char* name, u64 size);
        Shader& WithDynamicUniform(u32 binding, const char* name, u64 size, size_t count);
        Shader& WithStorage(u32 binding, const char* name, u64 size);
        Shader& WithDynamicStorage(u32 binding, const char* name, u64 size, size_t count);

        Shader& WithVertexType(u32 size);
        Shader& WithVertexAttribute(u32 offset, VertexDescription::AttributeType type);

        const Utils::StackArray<VertexBinding, MAX_UNIFORMS>& GetVertexBindings() const { return vertexBindings; }
        const Utils::StackArray<Uniform, MAX_UNIFORMS>& GetUniforms() const { return uniforms; }
        Utils::StackArray<VertexBinding, MAX_UNIFORMS>& GetVertexBindings() { return vertexBindings; }
        Utils::StackArray<Uniform, MAX_UNIFORMS>& GetUniforms() { return uniforms; }

        const char* GetPath() { return filePath; }
        PipelineConfig::PipelineStage GetStage() { return stage; } 

        u64 GetUniformSize() { return sizeOfUniforms; }

        void SetUniformType(u32 binding, const char* name, VkDescriptorType type, u64 size, size_t count = 1);

        const char* filePath;
        
        Utils::StackArray<Uniform, MAX_UNIFORMS> uniforms;
        Utils::StackArray<VertexBinding, MAX_UNIFORMS> vertexBindings;

        PipelineConfig::PipelineStage stage;
        u64 sizeOfUniforms = 0;
    };
}