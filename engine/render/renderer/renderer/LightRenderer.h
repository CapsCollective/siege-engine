//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include "../Core.h"
#include "../model/Model.h"

namespace Siege
{
class LightRenderer
{
public:

    LightRenderer();
    ~LightRenderer();

    void Initialise(const String& globalDataAttributeName, const uint64_t& globalDataSize);

    void Destroy();

    void DrawPointLight(const Siege::Vec3& position,
                        const float& radius,
                        const Siege::Vec4& colour,
                        const Siege::Vec4& ambientColor);

    void Render(VkCommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct PointLightVertex
    {
        alignas(16) Vec2 position;
    };

    struct LightUBO
    {
        Vec4 lightColor {1.f, 1.f, 1.f, 0.2f};
        alignas(16) Vec4 ambientLightColor {1.f, 1.f, 1.f, .02f};
        alignas(16) Vec3 position = {0.f};
        alignas(16) float radius = .05f;
    };

    Model lightModel;
    Material lightMaterial;

    Hash::StringId globalDataId;
    Hash::StringId lightDataId;

    StackArray<Vec2, Mesh::MAX_VERTICES> pointLightVertices;
    StackArray<uint32_t, Mesh::MAX_INDICES> pointLightIndices;
};
} // namespace Siege