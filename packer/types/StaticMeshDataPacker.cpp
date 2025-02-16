//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "StaticMeshDataPacker.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <resources/StaticMeshData.h>
#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <fstream>

REGISTER_TOKEN(SOURCE_PATH);
REGISTER_TOKEN(NODE_PATH);
REGISTER_TOKEN(FLIP_AXES);

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
                        aiMesh* mesh,
                        aiMatrix4x4t<ai_real> matrix,
                        OUT std::vector<Siege::BaseVertex>& vertices,
                        OUT std::vector<uint32_t>& indices)
{
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Siege::BaseVertex vertex {};

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

    auto it = std::max_element(indices.begin(), indices.end());
    uint32_t indexOffset = it != indices.end() ? *it + 1 : 0;
    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(indexOffset + face.mIndices[j]);
        }
    }
}

static void GetMeshesForNode(const aiScene* scene,
                             aiNode* node,
                             const Siege::String& requestPath,
                             Siege::String currentPath,
                             aiMatrix4x4t<ai_real> matrix,
                             OUT std::vector<Siege::BaseVertex>& vertices,
                             OUT std::vector<uint32_t>& indices)
{
    currentPath = currentPath + node->mName.C_Str();

    RequestPathStage nodePathStage = GetRequestPathStage(requestPath, currentPath);
    if (nodePathStage == NONE)
    {
        return;
    }

    CC_LOG_INFO("Reading node at {}", currentPath)

    if (nodePathStage == CHILD || nodePathStage == SELF)
    {
        matrix *= node->mTransformation;
    }

    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        Siege::String currentMeshPath = currentPath + '/' + mesh->mName.C_Str();
        RequestPathStage meshPathStage = GetRequestPathStage(requestPath, currentMeshPath);
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

void* PackStaticMeshFile(const Siege::String& filePath, const Siege::String& assetsPath)
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

    aiMatrix4x4t<ai_real> baseXform;
    auto flipAxesIt = attributes.find(TOKEN_FLIP_AXES);
    if (flipAxesIt != attributes.end())
    {
        if (flipAxesIt->second.Size() < 1 || flipAxesIt->second.Size() > 3)
        {
            CC_LOG_WARNING("FLIP_AXES attribute in .sm file contains invalid number of axes")
        }

        for (int32_t i = 0; i < flipAxesIt->second.Size(); ++i)
        {
            aiVector3t<ai_real> scale {1.f, 1.f, 1.f};
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
                CC_LOG_WARNING("Found invalid axis \"{}\" in FLIP_AXES attribute, ignoring", flipAxesIt->second[i])
                break;
            }
            aiMatrix4x4t<ai_real> scalingMat;
            baseXform *= aiMatrix4x4t<ai_real>::Scaling(scale, scalingMat);
        }
    }

    std::vector<Siege::BaseVertex> vertices;
    std::vector<uint32_t> indices;
    GetMeshesForNode(scene,
                     scene->mRootNode,
                     requestedNodePath,
                     '/',
                     baseXform,
                     vertices,
                     indices);

    Siege::StaticMeshData* staticMeshData = Siege::StaticMeshData::Create(indices, vertices);
    return staticMeshData;
}
