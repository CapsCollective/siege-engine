#ifndef A_DARK_DISCOMFORT_RENDERSYSTEM_H
#define A_DARK_DISCOMFORT_RENDERSYSTEM_H

#include "../utils/Maths.h"
#include "../entity/EntityPtr.h"
#include <variant>
#include <utility>
#include <map>

struct ModelData
{
    std::string modelPath;
    std::string texturePath;
};

struct LineData
{
    Vec3 trajectory;
    Colour colour;
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

    static RenderItem* Add(Entity* entity, const ModelData& modelData);

    static RenderItem* Add(Entity* entity, const ModelData& modelData, const Xform& transform);

    static void Remove(Entity* entity);

    static void DrawFrame();

    static void DrawFrame2D();

private:

    static std::map<EntityPtr<Entity>, RenderItem> renderItems;

    static std::map<EntityPtr<Entity>, RenderItem> renderItems2D;
};


#endif //A_DARK_DISCOMFORT_RENDERSYSTEM_H
