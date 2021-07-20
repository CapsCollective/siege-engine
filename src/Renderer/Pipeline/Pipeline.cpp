#include "Pipeline.h"

#include <fstream>
#include <string>
#include <iostream>

namespace SnekVk 
{
    Pipeline::Pipeline(char const* vertFilePath, char const* fragFilePath)
    {
        CreateGraphicsPipeline(vertFilePath, fragFilePath);
    }

    Pipeline::~Pipeline() {}

    FileData Pipeline::ReadFile(const char* filePath)
    {
        std::ifstream file { filePath, std::ios::ate | std::ios::binary };

        SNEK_ASSERT(file.is_open(), std::string("Could not find file: ") + filePath);

        size_t size = static_cast<size_t>(file.tellg());
        
        char* buffer = new char[size];
        
        file.seekg(0);
        
        file.read(buffer, size);

        file.close();
        
        return { buffer, size };
    }

    void Pipeline::CreateGraphicsPipeline(char const* vertFilePath, char const* fragFilePath)
    {
        auto vertCode = ReadFile(vertFilePath);
        auto fragCode = ReadFile(fragFilePath);

        std::cout << "Vert Size: " << vertCode.bufferSize << std::endl;
        std::cout << "Frag Size: " << fragCode.bufferSize << std::endl;

        DestroyFileData(vertCode);
        DestroyFileData(fragCode);
    }

    void DestroyFileData(FileData& data)
    {
        delete [] data.buffer;
    }
}