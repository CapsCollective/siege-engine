#include "Pipeline.h"

#include <fstream>
#include <string>
#include <iostream>

namespace SnekVk 
{
    Pipeline::Pipeline(
        VulkanDevice& device, 
        const char* vertFilePath, 
        const char* fragFilePath, 
        const PipelineConfigInfo& configInfo) : device{device}
    {
        CreateGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }

    Pipeline::~Pipeline() {}

    FileData Pipeline::ReadFile(const char* filePath)
    {
        std::ifstream file { filePath, std::ios::ate | std::ios::binary };

        SNEK_ASSERT(file.is_open(), std::string("Could not find file: ") + filePath);

        u32 size = static_cast<u32>(file.tellg());
        
        char* buffer = new char[size];
        
        file.seekg(0);
        
        file.read(buffer, size);

        file.close();
        
        return { buffer, size };
    }

    void Pipeline::CreateGraphicsPipeline(
        char const* vertFilePath, 
        char const* fragFilePath, 
        const PipelineConfigInfo& configInfo)
    {
        auto vertCode = ReadFile(vertFilePath);
        auto fragCode = ReadFile(fragFilePath);

        std::cout << "Vert Size: " << vertCode.bufferSize << std::endl;
        std::cout << "Frag Size: " << fragCode.bufferSize << std::endl;

        DestroyFileData(vertCode);
        DestroyFileData(fragCode);
    }

    void Pipeline::CreateShaderModule(struct FileData fileData, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = fileData.bufferSize;
        createInfo.pCode = reinterpret_cast<const u32*>(fileData.buffer);

        SNEK_ASSERT(vkCreateShaderModule(device.Device(), &createInfo, nullptr, shaderModule) == VK_SUCCESS, 
            "Failed to create shader module!");
    }

    PipelineConfigInfo Pipeline::DefaultPipelineConfig(u32 width, u32 height)
    {
        PipelineConfigInfo configInfo {};

        return configInfo;
    }

    void DestroyFileData(FileData& data)
    {
        delete [] data.buffer;
    }
}