//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "StaticMesh.h"

#include <resources/PackFile.h>
#include <resources/ResourceSystem.h>
#include <utils/Logging.h>

#include "Swapchain.h"
#include "resources/StaticMeshData.h"

namespace Siege::Vulkan
{
StaticMesh::StaticMesh(unsigned int vertexBufferSize,
                       unsigned int vertCount,
                       unsigned int indexBufferSize,
                       unsigned int idxCount,
                       unsigned int subMeshCount) :
    vertexCount {vertCount},
    indexCount {idxCount}
{
    CC_ASSERT(vertexCount > 0, "Cannot load in a file with no vertices!")
    CC_ASSERT(indexCount > 0, "Cannot load in a file with no indices!")

    CC_ASSERT(vertexCount < MAX_VERTICES, "The provided model has too many vertices!")
    CC_ASSERT(indexCount < MAX_INDICES, "The provided model has too many indices!")

    vertexBuffer = VertexBuffer(vertexBufferSize);
    indexBuffer = IndexBuffer(indexBufferSize);

    subMeshes = MHArray<SubMesh>(subMeshCount);
    materials = MHArray<Material*>(subMeshCount);
}

StaticMesh::StaticMesh(const char* filePath, Material* material)
{
    // TODO(Aryeh): How to extract material data from object files?
    PackFile* packFile = ResourceSystem::GetInstance().GetPackFile();
    std::shared_ptr<StaticMeshData> vertexData = packFile->FindData<StaticMeshData>(filePath);

    CC_ASSERT(vertexData->verticesCount > 0, "Cannot load in a file with no vertices!")
    CC_ASSERT(vertexData->indicesCount > 0, "Cannot load in a file with no indices!")

    CC_ASSERT(vertexData->verticesCount < MAX_VERTICES, "The provided model has too many vertices!")
    CC_ASSERT(vertexData->indicesCount < MAX_INDICES, "The provided model has too many indices!")

    vertexBuffer = VertexBuffer(sizeof(BaseVertex) * vertexData->verticesCount);
    vertexBuffer.Copy(vertexData->GetVertices(), sizeof(BaseVertex) * vertexData->verticesCount);

    indexBuffer = IndexBuffer(sizeof(unsigned int) * vertexData->indicesCount);
    indexBuffer.Copy(vertexData->GetIndices(), sizeof(unsigned int) * vertexData->indicesCount);

    vertexCount = vertexData->verticesCount;
    indexCount = vertexData->indicesCount;

    subMeshes = MHArray<SubMesh>(1);
    materials = MHArray<Material*>(1);

    subMeshes.Append({0, 0, 0, vertexCount, indexCount});
    materials.Append(material);
}

void StaticMesh::Swap(StaticMesh& other)
{
    auto tmpPerFrameVertexBuffers = std::move(vertexBuffer);
    auto tmpPerFrameIndexBuffers = std::move(indexBuffer);

    auto tmpVertexCount = vertexCount;
    auto tmpIndexCount = indexCount;

    auto tmpSubmeshes = subMeshes;
    auto tmpMaterials = materials;

    vertexBuffer = std::move(other.vertexBuffer);
    indexBuffer = std::move(other.indexBuffer);

    vertexCount = other.vertexCount;
    indexCount = other.indexCount;

    subMeshes = other.subMeshes;
    materials = other.materials;

    other.vertexBuffer = std::move(tmpPerFrameVertexBuffers);
    other.indexBuffer = std::move(tmpPerFrameIndexBuffers);

    other.vertexCount = tmpVertexCount;
    other.indexCount = tmpIndexCount;

    other.subMeshes = tmpSubmeshes;
    other.materials = tmpMaterials;
}

StaticMesh::~StaticMesh()
{
    Free();
}

void StaticMesh::Free()
{
    vertexBuffer.Free();
    indexBuffer.Free();
}

void StaticMesh::Bind(CommandBuffer& commandBuffer, uint64_t offset)
{
    vertexBuffer.Bind(commandBuffer, &offset);
}

void StaticMesh::BindIndexed(CommandBuffer& commandBuffer,
                             uint64_t vertexOffset,
                             uint64_t indexOffset)
{
    Bind(commandBuffer, vertexOffset);
    indexBuffer.Bind(commandBuffer, indexOffset);
}

} // namespace Siege::Vulkan