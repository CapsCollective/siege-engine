//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERSYSTEM_H
#define SIEGE_ENGINE_RENDERSYSTEM_H

#include <utils/Colour.h>
#include <utils/math/Maths.h>

#include <map>
#include <utility>
#include <variant>

#include "../entity/EntityPtr.h"

namespace Siege
{
struct ModelData
{
    String modelPath;
    String texturePath;
};

struct RenderItem
{
    RenderItem(ModelData modelData, const Xform& transform, bool isEnabled = true) :
        isEnabled(isEnabled),
        data(std::move(modelData)),
        transform(transform)
    {}

    bool isEnabled;
    ModelData data;
    const Xform& transform;
};

class RenderSystem
{
public:

    void Add(Entity* entity, const ModelData& modelData);

    void Add(Entity* entity, const ModelData& modelData, const Xform& transform);

    void Remove(Entity* entity);

    void DrawFrame();

    void DrawText2D(const String& text, int posX, int posY, int fontSize, Colour color);

    void DrawRectangle2D(int posX, int posY, int width, int height, Colour color);

private:

    std::map<EntityPtr<Entity>, RenderItem> renderItems;
};
} // namespace Siege

#endif // SIEGE_ENGINE_RENDERSYSTEM_H
