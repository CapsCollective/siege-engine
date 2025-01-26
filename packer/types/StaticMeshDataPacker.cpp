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
using Siege::Mat4;

REGISTER_TOKEN(SOURCE_PATH);

static void GetMeshData(const aiScene* scene, aiMesh* mesh, aiMatrix4x4t<ai_real> matrix, OUT std::vector<BaseVertex>& vertices, OUT std::vector<uint32_t>& indices)
{
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        BaseVertex vertex {};

        aiVector3t<ai_real> vert = mesh->mVertices[i];
        vert *= matrix;
        vertex.position = { vert.x, vert.y, vert.z};

        aiVector3t<ai_real> norm = mesh->mNormals[i];
        vertex.normal = { norm.x, norm.y, norm.z };

        aiColor4t<float>* color = mesh->mColors[0];
        vertex.color = color ? FColour{color[i].r, color[i].g, color[i].b, color[i].a} : FColour::White;

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

static void GetMeshesForNode(const aiScene* scene, aiNode* node, aiMatrix4x4t<ai_real> matrix, OUT std::vector<BaseVertex>& vertices, OUT std::vector<uint32_t>& indices)
{
    matrix *= node->mTransformation;
    CC_LOG_ERROR("NODE: \"{}\"", node->mName.C_Str())
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        CC_LOG_ERROR("MESH: \"{}\"", mesh->mName.C_Str())
        GetMeshData(scene, mesh, matrix, vertices, indices);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        GetMeshesForNode(scene, node->mChildren[i], matrix, vertices, indices);
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

    aiMatrix4x4t<ai_real> out;
    aiMatrix4x4t rotMat = aiMatrix4x4t<ai_real>::Rotation(1.570796f,{0.f, 1.f, 0.f}, out);
    aiMatrix4x4t rotMat2 = aiMatrix4x4t<ai_real>::Rotation(3.141593,{1.f, 0.f, 0.f}, out);

    std::vector<BaseVertex> vertices;
    std::vector<uint32_t> indices;
    GetMeshesForNode(scene, scene->mRootNode, aiMatrix4x4t<ai_real>(), vertices, indices);

    Siege::StaticMeshData* staticMeshData = Siege::StaticMeshData::Create(indices, vertices);
    return staticMeshData;
}
