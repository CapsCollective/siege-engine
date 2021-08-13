#pragma once

#include "../Buffer/Buffer.h"
#include "../Utils/Hash.h"

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
            glm::mat4 transform {1.0f};
            alignas(16) glm::vec3 color {0.0f, 0.0f, 0.0f};
        };

        /**
         * @brief The Vertex struct represents all the vertex data that'll be passed into our shaders.
         * Right now all we expect is a vector 2 for it's position in 2D space. 
         */
        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 color;
            glm::vec3 normal;
            glm::vec2 uv;

            /**
             * @brief Get a list of binding colorDescriptions for this Vertex. A binding description details
             * the size of the data being sent into the GPU and the type of data being sent in.
             * 
             * @return All vertex description information in the form of a std::array<VkVertexInputBindingDescription, 1> 
             */
            static std::array<VkVertexInputBindingDescription, 1> GetBindingDescriptions();

            /**
             * @brief Gets all attribute colorDescriptions for our vertex. This describes where the
             * data should be sent to and the format in which it is expected to arrive in. 
             * 
             * @return The attribute colorDescriptions in the form of a std::array<VkVertexInputAttributeDescription, 1>
             */
            static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

            bool operator==(const Vertex& other) const
            {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            };
        };

        struct Data 
        {
            Vertex* vertices {nullptr};
            u32 vertexCount {0};
            u32* indices {nullptr};
            u32 indexCount {0};
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

        void SetVertices(const Vertex* vertices, u32 vertexCount);

        /**
         * @brief Binds the model's buffers to the current command buffer. 
         * 
         * When using the same model across multiple objects, it is only necessary
         * to bind the vertex buffers once and simply pass in any dynamic data. 
         * This function handles any vertex-specific bindings that need to occur.
         * 
         * @param commandBuffer The command buffer being used for image recording. 
         */
        void Bind(VkCommandBuffer commandBuffer);

        /**
         * @brief Draws the current set vertices (and writes them to the currently bound vertex buffer).
         * 
         * @param commandBuffer The command buffer being used to draw the image
         */
        void Draw(VkCommandBuffer commandBuffer);

        private:

        /**
         * @brief Create a vertx buffer that can store our per-vertex information and upload it to the GPU. 
         * 
         * @param vertices An array of all vertices in model space.
         */
        void CreateVertexBuffers(const Vertex* vertices);
        void CreateIndexBuffer(const u32* indices);

        void LoadModelFromFile(Data& builder, const char* filePath);

        Buffer::Buffer vertexBuffer;
        u32 vertexCount;

        bool hasIndexBuffer = false;
        Buffer::Buffer indexBuffer;
        u32 indexCount;
    };
}
