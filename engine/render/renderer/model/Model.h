//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MODEL_H
#define SIEGE_ENGINE_MODEL_H

#include <utils/Hash.h>
#include <utils/math/mat/Mat4.h>

#include <unordered_map>

#include "../buffer/Buffer.h"
#include "../material/Material.h"
#include "../mesh/Mesh.h"

namespace Siege
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
        Mat4 transform;
        Mat4 normalMatrix;
    };

    // Placeholder - in case we need to add more unique 2D data
    struct Transform2D
    {
        Mat4 transform;
    };

    Model(const Mesh::MeshData& meshData);
    Model(const String& filePath);
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
    void Draw(VkCommandBuffer commandBuffer, const uint32_t& instance = 0);

    Material* GetMaterial()
    {
        return material;
    }
    void UpdateMesh(const Mesh::MeshData& meshData);
    void SetMesh(const Mesh::MeshData& meshData);
    void SetMaterial(Material* newMaterial)
    {
        material = newMaterial;
    }

    bool IsIndexed()
    {
        return modelMesh.HasIndexBuffer();
    }

private:

    void LoadModelFromFile(const String& filePath);

    Mesh modelMesh;
    Material* material {nullptr};
};
} // namespace Siege

#endif
