#pragma once

#include "../Core.h"
#include "../Pipeline/Pipeline.h"
#include "../Buffer/Buffer.h"

namespace SnekVk
{
    class Shader
    {
        public: 

        Shader();
        ~Shader();

        struct VertexBinding
        {
            static constexpr size_t MAX_VERTEX_ATTRIBUTES = 10;
            size_t attributeCount = 0;
            u32 vertexStride = 0;
            VertexDescription::Attribute attributes[MAX_VERTEX_ATTRIBUTES];
        };

        template<typename T, size_t S>
        struct Storage
        {
            static constexpr size_t MAX_COUNT = S;
            size_t count = 0; 
            T data[MAX_COUNT];
        };

        struct VertexStorage
        {
            static constexpr size_t MAX_VERTEX_BINDINGS = 5;
            size_t bindingCount = 0; 
            VertexBinding bindings[MAX_VERTEX_BINDINGS];
        };

        void AddVertexAttribute(u32 binding, u32 offset, VertexDescription::AttributeType type);
        void SetVertexInputSize(u32 binding, u32 offset);

        private:

    };
}