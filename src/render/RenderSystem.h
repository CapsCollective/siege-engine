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

#endif // A_DARK_DISCOMFORT_RENDERSYSTEM_H
