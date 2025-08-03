//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "StaticMeshDataPacker.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <resources/PackFileData.h>
#include <resources/StaticMeshData.h>
#include <utils/BinarySerialisation.h>
#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <fstream>

#include "../PackerUtils.h"

enum RequestPathStage
{
    PARENT,
    CHILD,
    SELF,
    NONE,
};

static RequestPathStage GetRequestPathStage(const Siege::String& requestPath,
                                            const Siege::String& currentPath)
{
    if (currentPath.Size() == requestPath.Size())
    {
        return SELF;
    }

    if (requestPath.BeginsWith(currentPath))
    {
        return PARENT;
    }

    if (currentPath.BeginsWith(requestPath))
    {
        return CHILD;
    }

    return NONE;
}

static void GetMeshData(const aiScene* scene,
                        const aiMesh* mesh,
                        const Siege::Mat4& mat,
                        OUT std::vector<Siege::BaseVertex>& vertices,
                        OUT std::vector<uint32_t>& indices)
{
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Siege::BaseVertex vertex {};

        aiVector3t<ai_real> vert = mesh->mVertices[i];
        vertex.position = mat * Siege::Vec4(vert.x, vert.y, vert.z, 1.f);

        aiVector3t<ai_real> norm = mesh->mNormals[i];
        vertex.normal = {norm.x, norm.y, norm.z};

        const aiColor4t<float>* color = mesh->mColors[0];
        vertex.color = color ? Siege::FColour {color[i].r, color[i].g, color[i].b, color[i].a} :
                               Siege::FColour::White;

        if (aiVector3t<ai_real>* uv = mesh->mTextureCoords[0])
        {
            vertex.uv = {uv[i].x, uv[i].y};
        }

        vertices.push_back(vertex);
    }

    const auto it = std::max_element(indices.begin(), indices.end());
    const uint32_t indexOffset = it != indices.end() ? *it + 1 : 0;
    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(indexOffset + face.mIndices[j]);
        }
    }
}

static void GetMeshesForNode(const aiScene* scene,
                             const aiNode* node,
                             const Siege::String& requestPath,
                             Siege::String currentPath,
                             Siege::Mat4 matrix,
                             OUT std::vector<Siege::BaseVertex>& vertices,
                             OUT std::vector<uint32_t>& indices)
{
    currentPath = currentPath + node->mName.C_Str();

    const RequestPathStage nodePathStage = GetRequestPathStage(requestPath, currentPath);
    if (nodePathStage == NONE)
    {
        return;
    }

    CC_LOG_INFO("Reading node at {}", currentPath)

    if (nodePathStage == CHILD || nodePathStage == SELF)
    {
        matrix *= AssimpMat4ToMat4(node->mTransformation);
    }

    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        Siege::String currentMeshPath = currentPath + '/' + mesh->mName.C_Str();
        const RequestPathStage meshPathStage = GetRequestPathStage(requestPath, currentMeshPath);
        if (meshPathStage == NONE)
        {
            continue;
        }

        CC_LOG_INFO("Reading mesh at {}", currentMeshPath)
        GetMeshData(scene, mesh, matrix, vertices, indices);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        GetMeshesForNode(scene,
                         node->mChildren[i],
                         requestPath,
                         currentPath + '/',
                         matrix,
                         vertices,
                         indices);
    }
};

Siege::PackFileData* PackStaticMeshFile(const Siege::String& filePath,
                                        const Siege::String& assetsPath)
{
    Siege::String contents = Siege::FileSystem::Read(filePath);
    std::map<Siege::Token, Siege::String> attributes =
        Siege::FileSystem::ParseAttributeFileData(contents);

    if (attributes.empty())
    {
        CC_LOG_WARNING("Received empty static mesh file at path \"{}\"", filePath);
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

    auto nodePathIt = attributes.find(TOKEN_NODE_PATH);
    Siege::String requestedNodePath = nodePathIt != attributes.end() ? nodePathIt->second : "/";

    CC_LOG_INFO("Reading static mesh for file {} with node path {}", it->second, requestedNodePath)

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

    Siege::StaticMeshData staticMeshData;
    GetMeshesForNode(scene,
                     scene->mRootNode,
                     requestedNodePath,
                     '/',
                     baseXform,
                     staticMeshData.vertices,
                     staticMeshData.indices);

    Siege::BinarySerialisation::Buffer dataBuffer;
    Siege::BinarySerialisation::serialise(dataBuffer,
                                          staticMeshData,
                                          Siege::BinarySerialisation::SERIALISE);

    char* data = reinterpret_cast<char*>(dataBuffer.data.data());
    Siege::PackFileData* fileData = Siege::PackFileData::Create(data, dataBuffer.data.size());
    return fileData;
}
