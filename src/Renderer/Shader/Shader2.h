#pragma once

#include "../Core.h"
#include "../Pipeline/Pipeline.h"
#include <map>

namespace SnekVk 
{
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
        VkDescriptorType type;
    };


    struct VertexBinding
    {
        static constexpr size_t MAX_VERTEX_ATTRIBUTES = 10;
        Utils::StackArray<VertexDescription::Attribute, MAX_VERTEX_ATTRIBUTES> attributes;
        u32 vertexStride = 0;
    };

    class Shader2 
    {
        public: 

        static constexpr size_t MAX_UNIFORMS_PER_STAGE = 5; 

        

        private: 

        
        std::map<Utils::StringId, Uniform> uniforms;
    };

    class ShaderBuilder
    {
        typedef Utils::StackArray<VertexDescription::Attribute, 5> attributes;
        
        public:

        ShaderBuilder();
        ~ShaderBuilder();

        static constexpr size_t MAX_UNIFORMS = 5;

        static ShaderBuilder CreateShaderBuilder();

        ShaderBuilder& FromShader(const char* filePath);
        ShaderBuilder& WithStage(PipelineConfig::PipelineStage stage);
        
        ShaderBuilder& WithUniform(u32 binding, const char* name, u64 size);
        ShaderBuilder& WithDynamicUniform(u32 binding, const char* name, u64 size);
        ShaderBuilder& WithStorage(u32 binding, const char* name, u64 size);
        ShaderBuilder& WithDynamicStorage(u32 binding, const char* name, u64 size);

        ShaderBuilder& WithVertexType(u32 size);
        ShaderBuilder& WithVertexAttribute(u32 offset, VertexDescription::AttributeType type);

        Utils::StackArray<VertexBinding, MAX_UNIFORMS> GetVertexBindings() { return vertexBindings; }
        Utils::StackArray<Uniform, MAX_UNIFORMS> GetUniforms() { return uniforms; }

        u64 GetUniformSize() { return sizeOfUniforms; }

        private:

        void SetUniformType(u32 binding, const char* name, u64 size, VkDescriptorType type);

        const char* filePath;
        
        Utils::StackArray<Uniform, MAX_UNIFORMS> uniforms;
        Utils::StackArray<VertexBinding, MAX_UNIFORMS> vertexBindings;

        PipelineConfig::PipelineStage stage;
        u64 sizeOfUniforms = 0;
    };
}