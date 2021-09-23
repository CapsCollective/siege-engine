#include "Shader.h"

namespace SnekVk
{
    Shader::Shader()
    {

    }

    Shader::Shader(const char* filePath, PipelineConfig::PipelineStage stage) 
            : filePath{filePath}, stage{stage}
    {
        
    }

    Shader::~Shader()
    {

    }

    void Shader::SetVertexInputSize(u32 binding, u32 stride)
    {
        SNEK_ASSERT(binding < vertexStorage.MAX_COUNT, 
                "Cannot assign attributes to more than 5 bindings!");
        
        vertexStorage.data[binding].vertexStride = stride;
    }

    void Shader::SetShaderPath(const char* filePath)
    {
        this->filePath = filePath;
    }

    void Shader::AddVertexAttribute(u32 binding, u32 offset, VertexDescription::AttributeType type)
    {
        SNEK_ASSERT(vertexStorage.count < vertexStorage.MAX_COUNT, 
                std::string("Too many vertex attributes assigned. Max is ")
                .append(std::to_string(vertexStorage.MAX_COUNT)).c_str()); 
        
        SNEK_ASSERT(binding < vertexStorage.MAX_COUNT, 
                "Cannot assign attributes to more than 5 bindings!");
        
        u32 attributeCount = vertexStorage.data[binding].attributeCount;

        if (attributeCount == 0) vertexStorage.count++;

        vertexStorage.data[binding].attributes[attributeCount] = {offset, type};
        vertexStorage.data[binding].attributeCount++;
    }

    void Shader::SetUniformStruct(u32 location, u32 binding, const char* name, u64 size)
    {
        SNEK_ASSERT(uniformStructs.count < uniformStructs.MAX_COUNT, 
            "Cannot assign more than 5 uniform structs per shader!");

        Utils::StringId strId = INTERN_STR(name);

        u32 uniformIdx = GetUniformStructIdx(strId);

        if (uniformIdx == -1) uniformIdx = uniformStructs.count++;

        uniformStructs.data[uniformIdx] = {strId, location, binding, size};

        std::cout << "Added uniform: " << name << " at index: " << uniformIdx << std::endl;
    }

    void Shader::SetUniformStruct(u32 location, u32 binding, Utils::StringId strId, u64 size)
    {
        SNEK_ASSERT(uniformStructs.count < uniformStructs.MAX_COUNT, 
            "Cannot assign more than 5 uniform structs per shader!");
        
        u32 uniformIdx = GetUniformStructIdx(strId);

        if (uniformIdx == -1) uniformIdx = uniformStructs.count++;

        uniformStructs.data[uniformIdx] = {strId, location, binding, size};

        std::cout << "Added uniform at index: " << uniformIdx << std::endl;
    }

    u32 Shader::GetUniformStructIdx(const char* name)
    {
        return GetUniformStructIdx(INTERN_STR(name));
    }

    u32 Shader::GetUniformStructIdx(Utils::StringId strId)
    {
        for (size_t i = 0; i < uniformStructs.count; i++)
        {
            if (strId == uniformStructs.data[i].id) return i;
        }

        return -1;
    }
}