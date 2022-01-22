#ifndef A_DARK_DISCOMFORT_RENDERSYSTEM_H
#define A_DARK_DISCOMFORT_RENDERSYSTEM_H

#include <map>
#include <utility>
#include <variant>

#include "../entity/EntityPtr.h"
#include "../utils/Colour.h"
#include "../utils/Maths.h"

struct ModelData
{
    std::string modelPath;
    std::string texturePath;
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

    static void DrawText2D(const std::string& text, int posX, int posY, int fontSize, Colour color);

    static void DrawRectangle2D(int posX, int posY, int width, int height, Colour color);

private:

    static std::map<EntityPtr<Entity>, RenderItem> renderItems;

    static std::map<EntityPtr<Entity>, class RenderItem2D> renderItems2D;
};

#endif // A_DARK_DISCOMFORT_RENDERSYSTEM_H
