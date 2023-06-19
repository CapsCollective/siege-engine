//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MODEL_H
#define SIEGE_ENGINE_MODEL_H

#include <utils/Colour.h>
#include <utils/Hash.h>
#include <utils/math/mat/Mat4.h>

#include "../buffer/Buffer.h"
#include "render/renderer/platform/vulkan/Material.h"
#include "render/renderer/platform/vulkan/Mesh.h"

#include <unordered_map>

namespace Siege
{
/**
 * @brief The model class contains all rendering data required by our shaders. Models
 * contain all relevant vertex information, alongside a buffer containing our vertices.
 */
class Model
{
public:

    // TODO(Aryeh): Move model loading into a separate file? That way we can separate the Vertex
    // TODO(Aryeh): from this class
    struct ModelVertex3D
    {
        Vec3 position;
        FColour color;
        Vec3 normal;
        Vec2 uv;
    };

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

    Model(const Vulkan::Mesh::VertexData& vertices, const Vulkan::Mesh::IndexData& indices);
    Model(Vulkan::Mesh&& newMesh);
    Model(const String& filePath);
    Model();
    ~Model();

    void DestroyModel();

    // Model is not copyable - we just delete the copy constructors.
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void Bind(Vulkan::CommandBuffer& commandBuffer, uint32_t frameIndex);
    void BindIndexed(Vulkan::CommandBuffer& commandBuffer, uint32_t frameIndex);

    void Draw(Vulkan::CommandBuffer& commandBuffer,
              uint32_t currentFrame,
              const uint32_t& instances = 0);
    void DrawIndexed(Vulkan::CommandBuffer& commandBuffer,
                     const uint32_t frameIndex,
                     const uint32_t& instance = 0);

    Vulkan::Material* GetMaterial()
    {
        return material;
    }

    void UpdateMeshIndexed(uint32_t currentFrame,
                           const Vulkan::Mesh::VertexData& vertexData,
                           const Vulkan::Mesh::IndexData& indices);
    void UpdateMesh(uint32_t currentFrame, const Vulkan::Mesh::VertexData& vertexData);
    void SetMesh(Vulkan::Mesh&& newMesh);
    void SetMaterial(Vulkan::Material* newMaterial)
    {
        material = newMaterial;
    }

private:

    void LoadModelFromFile(const String& filePath);

    Vulkan::Mesh mesh;
    Vulkan::Material* material {nullptr};
};

inline bool operator==(const Model::ModelVertex3D& left, const Model::ModelVertex3D& right)
{
    return left.position == right.position && left.color == right.color &&
           left.normal == right.normal && left.uv == right.uv;
}
} // namespace Siege

#endif
