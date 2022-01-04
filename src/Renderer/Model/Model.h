#pragma once

#include "../Buffer/Buffer.h"
#include "../Utils/Hash.h"
#include "../Mesh/Mesh.h"
#include "../Material/Material.h"

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

        struct Transform
        {
            glm::mat4 transform;
            glm::mat4 normalMatrix;
        };

        // Placeholder - in case we need to add more unique 2D data
        struct Transform2D
        {
            glm::mat4 transform;
        };

        Model(const Mesh::MeshData& meshData);
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
        void Draw(VkCommandBuffer commandBuffer, u32 instance = 0);

        Material* GetMaterial() { return material; }
        void UpdateMesh(const Mesh::MeshData& meshData);
        void SetMaterial(Material* newMaterial) { material = newMaterial; }

        bool IsIndexed() { return modelMesh.HasIndexBuffer(); }

        private:

        void LoadModelFromFile(const char* filePath);

        Mesh modelMesh;
        Material* material{nullptr};
    };
}
