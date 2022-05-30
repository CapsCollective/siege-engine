#pragma once

#include "../Core.h"
#include "../model/Model.h"
#include "../utils/Math.h"

namespace Siege
{
    class ModelRenderer
    {
        public:

        ModelRenderer();
        ~ModelRenderer();

        void Initialise(const char* globalDataAttributeName, const u64& globalDataSize);
        void Destroy();

        void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);

        void Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);

        void Flush();

        void RecreateMaterials();

        private:

        // TODO(Aryeh): Make this configurable via macros
        static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

        Utils::StringId globalDataId;
        Utils::StringId transformId;

        Utils::StackArray<Model::Transform, MAX_OBJECT_TRANSFORMS> transforms;
        Utils::StackArray<Model*, MAX_OBJECT_TRANSFORMS> models;

        Material* currentMaterial {nullptr}; 
        Model* currentModel {nullptr};
    };
}