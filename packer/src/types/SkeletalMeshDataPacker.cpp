//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SkeletalMeshDataPacker.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <resources/SkeletalMeshData.h>
#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <fstream>

#include "../PackerUtils.h"
#include "StaticMeshDataPacker.h"

static void GetMeshData(const aiScene* scene,
                        aiMesh* mesh,
                        Siege::Mat4 mat,
                        OUT Siege::SkeletalMeshData& meshData)
{
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Siege::SkinnedVertex vertex {};
        vertex.bones = {-1, -1, -1, -1};
        vertex.weights = {0.f, 0.f, 0.f, 0.f };

        aiVector3t<ai_real> vert = mesh->mVertices[i];
        vertex.position = mat * Siege::Vec4(vert.x, vert.y, vert.z, 1.f);

        aiVector3t<ai_real> norm = mesh->mNormals[i];
        vertex.normal = {norm.x, norm.y, norm.z};

        aiColor4t<float>* color = mesh->mColors[0];
        vertex.color = color ? Siege::FColour {color[i].r, color[i].g, color[i].b, color[i].a} :
                               Siege::FColour::White;

        if (aiVector3t<ai_real>* uv = mesh->mTextureCoords[0])
        {
            vertex.uv = {uv->x, uv->y};
        }

        meshData.vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
        {
            meshData.indices.push_back(face.mIndices[j]);
        }
    }

    for (uint32_t i = 0; i < mesh->mNumBones; i++)
    {
        aiBone* currentBone = mesh->mBones[i];

        uint32_t boneId;
        Siege::String boneName(currentBone->mName.C_Str());

        auto it = meshData.bones.find(boneName);
        if (it != meshData.bones.end())
        {
            boneId = it->second.id;
        }
        else
        {
            boneId = meshData.bones.size();

            Siege::Bone newBone;
            newBone.id = boneId;
            newBone.bindMatrix = AssimpMat4ToMat4(currentBone->mOffsetMatrix);
            meshData.bones.emplace(boneName, newBone);
        }

        CC_LOG_INFO("Reading bone {} with id {}", boneName, boneId)

        for (int32_t j = 0; j < currentBone->mNumWeights; ++j)
        {
            aiVertexWeight weight = currentBone->mWeights[j];
            Siege::SkinnedVertex vertex = meshData.vertices[weight.mVertexId];
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

Siege::PackFileData* PackSkeletalMeshFile(const Siege::String& filePath, const Siege::String& assetsPath)
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

    Siege::Mat4 baseXform = Siege::Mat4::Identity();
    auto flipAxesIt = attributes.find(TOKEN_FLIP_AXES);
    if (flipAxesIt != attributes.end())
    {
        if (flipAxesIt->second.Size() < 1 || flipAxesIt->second.Size() > 3)
        {
            CC_LOG_WARNING("FLIP_AXES attribute in .sm file contains invalid number of axes")
        }

        for (int32_t i = 0; i < flipAxesIt->second.Size(); ++i)
        {
            Siege::Vec4 scale = {1.f, 1.f, 1.f, 1.f};
            switch (flipAxesIt->second[i])
            {
                case 'x':
                    scale.x *= -1;
                    break;
                case 'y':
                    scale.y *= -1;
                    break;
                case 'z':
                    scale.z *= -1;
                    break;
                default:
                    CC_LOG_WARNING("Found invalid axis \"{}\" in FLIP_AXES attribute, ignoring",
                                   flipAxesIt->second[i])
                    break;
            }
            baseXform *= Siege::Mat4::Scale(scale);
        }
    }

    Siege::SkeletalMeshData skeletalMeshData;
    GetMeshData(scene, mesh, baseXform, skeletalMeshData);

    Siege::BinarySerialisation::Buffer dataBuffer;
    Siege::BinarySerialisation::serialise(dataBuffer,
                                          skeletalMeshData,
                                          Siege::BinarySerialisation::SERIALISE);

    char* data = reinterpret_cast<char*>(dataBuffer.data.data());
    Siege::PackFileData* fileData = Siege::PackFileData::Create(data, dataBuffer.data.size());
    return fileData;
}
