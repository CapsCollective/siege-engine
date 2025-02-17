//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SkeletalMeshDataPacker.h"

#include <resources/SkeletalMeshData.h>
#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <algorithm>
#include <fstream>

#include "StaticMeshDataPacker.h"

static inline Siege::Mat4 ConvertMatrix(const aiMatrix4x4& from)
{
    Siege::Mat4 to;
    //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}

static void GetMeshData(const aiScene* scene,
                        aiMesh* mesh,
                        aiMatrix4x4t<ai_real> matrix,
                        OUT std::vector<Siege::SkinnedVertex>& vertices,
                        OUT std::vector<uint32_t>& indices,
                        OUT std::map<Siege::String, Siege::Bone>& bones)
{
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Siege::SkinnedVertex vertex {};
        vertex.bones = {-1, -1, -1, -1};
        vertex.weights = {0.f, 0.f, 0.f, 0.f };

        aiVector3t<ai_real> vert = mesh->mVertices[i];
        vert *= matrix;
        vertex.position = {vert.x, vert.y, vert.z};

        aiVector3t<ai_real> norm = mesh->mNormals[i];
        vertex.normal = {norm.x, norm.y, norm.z};

        aiColor4t<float>* color = mesh->mColors[0];
        vertex.color = color ? Siege::FColour {color[i].r, color[i].g, color[i].b, color[i].a} :
                               Siege::FColour::White;

        if (aiVector3t<ai_real>* uv = mesh->mTextureCoords[0])
        {
            vertex.uv = {uv->x, uv->y};
        }

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    for (uint32_t i = 0; i < mesh->mNumBones; i++)
    {
        aiBone* currentBone = mesh->mBones[i];

        uint32_t boneId;
        Siege::String boneName(currentBone->mName.C_Str());

        auto it = bones.find(boneName);
        if (it != bones.end())
        {
            boneId = it->second.id;
        }
        else
        {
            boneId = bones.size();

            Siege::Bone newBone;
            newBone.id = boneId;
            newBone.bindMatrix = ConvertMatrix(currentBone->mOffsetMatrix);
            bones.emplace(boneName, newBone);
        }

        CC_LOG_INFO("Reading bone {} with id {}", boneName, boneId)

        for (int32_t j = 0; j < currentBone->mNumWeights; ++j)
        {
            aiVertexWeight weight = currentBone->mWeights[j];
            Siege::SkinnedVertex vertex = vertices[weight.mVertexId];
            for (int32_t k = 0; k < 4; ++k)
            {
                if (vertex.bones[k] < 0)
                {
                    vertex.bones[k] = static_cast<float>(boneId);
                    vertex.weights[k] = weight.mWeight;
                    break;
                }
            }
        }
    }
}

static aiMesh* FindMeshAtPath(const aiScene* scene, aiNode* node, const Siege::String& requestPath, Siege::String currentPath)
{
    currentPath = currentPath + node->mName.C_Str();
    CC_LOG_INFO("Reading node at {}", currentPath)

    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Siege::String currentMeshPath = currentPath + '/' + mesh->mName.C_Str();
        CC_LOG_INFO("Reading mesh at {}", currentMeshPath)

        if (currentMeshPath != requestPath) continue;
        return mesh;
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        aiMesh* mesh = FindMeshAtPath(scene, node->mChildren[i], requestPath, currentPath + '/');
        if (mesh) return mesh;
    }

    return nullptr;
}

void* PackSkeletalMeshFile(const Siege::String& filePath, const Siege::String& assetsPath)
{
    Siege::String contents = Siege::FileSystem::Read(filePath);
    std::map<Siege::Token, Siege::String> attributes = Siege::FileSystem::ParseAttributeFileData(contents);

    if (attributes.empty())
    {
        CC_LOG_WARNING("Received empty skeletal mesh file at path \"{}\"", filePath);
        return nullptr;
    }

    auto it = attributes.find(TOKEN_SOURCE_PATH);
    if (it == attributes.end())
    {
        CC_LOG_WARNING("Failed to find SOURCE_PATH attribute in .sm file at path \"{}\"", filePath)
        return nullptr;
    }

    Siege::String modelPath = assetsPath + '/' + it->second;
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(modelPath,
                          aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                              aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (!scene)
    {
        CC_LOG_WARNING("Failed to read file at path \"{}\"", modelPath)
        return nullptr;
    }

    if (!scene->HasMeshes())
    {
        CC_LOG_WARNING("File at path \"{}\" contains no meshes", modelPath)
        return nullptr;
    }

    aiMesh* mesh = nullptr;
    auto nodePathIt = attributes.find(TOKEN_NODE_PATH);
    if (nodePathIt != attributes.end())
    {
        Siege::String requestedNodePath = nodePathIt->second;
        CC_LOG_INFO("Reading skeletal mesh for file {} with path {}", it->second, requestedNodePath)
        mesh = FindMeshAtPath(scene, scene->mRootNode, requestedNodePath, '/');
        if (!mesh)
        {
            CC_LOG_WARNING("Failed to find mesh at path \"{}\"", requestedNodePath)
            return nullptr;
        }
    }
    else
    {
        CC_LOG_INFO("Reading skeletal mesh for file {}", it->second)
        mesh = scene->mMeshes[0];
    }

    std::vector<Siege::SkinnedVertex> vertices;
    std::vector<uint32_t> indices;
    std::map<Siege::String, Siege::Bone> bones;
    GetMeshData(scene, mesh, aiMatrix4x4t<ai_real>(), vertices, indices, bones);

    Siege::SkeletalMeshData* skeletalMeshData = Siege::SkeletalMeshData::Create(indices, vertices, bones);
    return skeletalMeshData;
}
