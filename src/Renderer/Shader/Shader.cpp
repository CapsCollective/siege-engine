#include "Shader.h"

namespace SnekVk
{
    Shader::Shader() {}

    Shader::~Shader() {}

    Shader Shader::BuildShader()
    {
        return Shader();
    }

    Shader& Shader::FromShader(const char* filePath)
    {
        this->filePath = filePath;

        return *this;
    }

    Shader& Shader::WithStage(PipelineConfig::PipelineStage stage) 
    {
        this->stage = stage;
        return *this;
    }

    Shader& Shader::WithUniform(u32 binding, const char* name, u64 size)
    {
        SetUniformType(binding, name, size, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

        std::cout << "Added uniform " << name << " for binding: " << binding << std::endl;

        return *this;
    }

    Shader& Shader::WithDynamicUniform(u32 binding, const char* name, u64 size)
    {
        SetUniformType(binding, name, size, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC);

        std::cout << "Added dynamic uniform " << name << " for binding: " << binding << std::endl;

        return *this;
    }

    Shader& Shader::WithStorage(u32 binding, const char* name, u64 size)
    {
        SetUniformType(binding, name, size, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

        std::cout << "Added storage " << name << "for binding: " << binding << std::endl;

        return *this;
    }

    Shader& Shader::WithDynamicStorage(u32 binding, const char* name, u64 size)
    {
        SetUniformType(binding, name, size, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC);

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

        SNEK_ASSERT(index >= 0, "A vertex type must be added before creating attributes!");

        auto& binding = vertexBindings.Get(index);
        auto& attributes = binding.attributes;

        attributes.Append({offset, type});

        std::cout << "Added new vertex attribute for binding " << index << std::endl;
        std::cout << "Binding now has " << binding.attributes.Count() << " attributes" << std::endl;

        return *this;
    }

    void Shader::SetUniformType(u32 binding, const char* name, u64 size, VkDescriptorType type)
    {
        SNEK_ASSERT(uniforms.Count() <= uniforms.Size(), 
            std::string("ERROR: Maximum number of uniforms have been reached. Maximum is " 
            + std::to_string(uniforms.Size())).c_str());

        Utils::StringId strId = INTERN_STR(name);

        uniforms.Append({strId, binding, size, type});

        sizeOfUniforms += size;
    }
}