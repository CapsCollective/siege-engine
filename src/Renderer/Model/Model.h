#pragma once

#include "../Buffer/Buffer.h"
#include "../Utils/Hash.h"
#include "../Mesh/Mesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>

namespace SnekVk 
{
    /**
     * @brief The model class contains all rendering data required by our shaders. Models
     * contain all relevant vertex information, alongside a buffer containing our vertices. 
     */
    class Model
    {
        public:

        struct PushConstantData
        {
            glm::mat4 transform {1.f};
            glm::mat4 normalMatrix {1.f};
        };

        struct Data 
        {
            Vertex* vertices {nullptr};
            u32 vertexCount {0};
            u32* indices {nullptr};
            u32 indexCount {0};
        };

        struct Transform
        {
            glm::mat4 transform;
            glm::mat4 normalMatrix;
        };

        // 'Structors

        /**
         * @brief Constructor for creating a new model.
         * 
         * @param configData a struct specifying the config information required to create a model
         */
        Model(const Data& configData);
        Model(const char* filePath);
        Model();
        ~Model();

        void DestroyModel();

        // Model is not copyable - we just delete the copy constructors.
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        void Bind(VkCommandBuffer commandBuffer);

        /**
         * @brief Draws the current set vertices (and writes them to the currently bound vertex buffer).
         * 
         * @param commandBuffer The command buffer being used to draw the image
         */
        void Draw(VkCommandBuffer commandBuffer);

        private:

        void LoadModelFromFile(const Data& builder, const char* filePath);

        Mesh modelMesh;
    };
}
