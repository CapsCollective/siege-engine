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

    void Initialise(const String& globalDataAttributeName, const u64& globalDataSize);

    void Destroy();

    void DrawPointLight(const glm::vec3& position,
                        const float& radius,
                        const glm::vec4& colour,
                        const glm::vec4& ambientColor);

    void Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    Model lightModel {};
    Material lightMaterial {};

    Utils::StringId globalDataId {};

    Utils::StackArray<glm::vec2, Mesh::MAX_VERTICES> pointLightVertices;
    Utils::StackArray<u32, Mesh::MAX_INDICES> pointLightIndices;
};
} // namespace Siege