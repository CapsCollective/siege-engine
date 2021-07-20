#pragma once

#include "../Renderer.h"

namespace SnekVk 
{
    class Pipeline 
    {
        public:
            Pipeline(char const* vertFilePath, char const* fragFilePath);

            ~Pipeline();
        private:

            static struct FileData ReadFile(const char* filePath);
            void CreateGraphicsPipeline(char const* vertFilePath, char const* fragFilePath);
    };

    struct FileData
    {
        char* buffer = nullptr;
        size_t bufferSize = 0;
    };

    void DestroyFileData(FileData& data);
}