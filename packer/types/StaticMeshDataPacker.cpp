//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "StaticMeshDataPacker.h"

#include <utils/FileSystem.h>
#include <utils/Logging.h>
#include <resources/StaticMeshData.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <algorithm>
#include <fstream>

using Siege::BaseVertex;
using Siege::FColour;
using Siege::Vec2;
using Siege::Vec3;

REGISTER_TOKEN(SOURCE_PATH);

static Siege::StaticMeshData* GetMeshData(const aiScene* scene, aiMesh* mesh)
{
    std::vector<BaseVertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<BaseVertex, uint32_t> uniqueVertices {};

    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        BaseVertex vertex {};

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal = Vec3 { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        if (aiColor4t<float>* color = mesh->mColors[0])
        {
            vertex.color = FColour {color[i].r, color[i].g, color[i].b, color[i].a};
        }
        else
        {
            vertex.color = FColour::White;
        }

        if (aiVector3t<ai_real>* uv = mesh->mTextureCoords[0])
        {
            vertex.uv.x = static_cast<float>(uv[i].x);
            vertex.uv.y = static_cast<float>(uv[i].y);
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
    Siege::StaticMeshData* staticMeshData = Siege::StaticMeshData::Create(indices, vertices);
    return staticMeshData;
}

static std::vector<Siege::StaticMeshData*> GetMeshesForNode(const aiScene* scene, aiNode* node)
{
    std::vector<Siege::StaticMeshData*> meshData;
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Siege::StaticMeshData* staticMeshData = GetMeshData(scene, mesh);
        if (staticMeshData)
        {
            meshData.push_back(staticMeshData);
        }
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        std::vector<Siege::StaticMeshData*> staticMeshData = GetMeshesForNode(scene, node->mChildren[i]);
        if (!staticMeshData.empty())
        {
            meshData.insert(meshData.end(), staticMeshData.begin(), staticMeshData.end());
        }
    }

    return meshData;
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
        CC_LOG_ERROR("Failed to find SOURCE_PATH attribute in .sm file at path \"{}\"", filePath)
        return nullptr;
    }

    Siege::String modelPath = assetsPath + '/' + it->second;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelPath,
                                             aiProcess_CalcTangentSpace |
                                                 aiProcess_Triangulate |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_SortByPType);
    if (!scene)
    {
        CC_LOG_ERROR("Failed to read file at path \"{}\"", modelPath)
        return nullptr;
    }
    std::vector<Siege::StaticMeshData*> staticMeshData = GetMeshesForNode(scene, scene->mRootNode);
    return staticMeshData[0];
}
