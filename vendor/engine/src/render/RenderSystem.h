#ifndef A_DARK_DISCOMFORT_RENDERSYSTEM_H
#define A_DARK_DISCOMFORT_RENDERSYSTEM_H

#include "../utils/Maths.h"
#include "../entity/EntityPtr.h"
#include <utility>
#include <vector>

struct ModelData
{
    ModelData(std::string modelPath, std::string texturePath) :
        modelPath(std::move(modelPath)),
        texturePath(std::move(texturePath))
    {}

    std::string modelPath;
    std::string texturePath;
};

struct RenderItem
{
    RenderItem(Entity* entity, const std::string& modelPath, const std::string& texturePath) :
        entity(entity),
        modelData(modelPath, texturePath)
    {}

    EntityPtr<Entity> entity;
    ModelData modelData;
};


class RenderSystem
{
public:

    static void Add(Entity* entity, const std::string& modelPath, const std::string& texturePath);

    static RenderItem* Get(Entity* entity);

    static void Remove(Entity* entity);

    static void DrawFrame();

private:

    static std::vector<RenderItem> renderItems;
};


#endif //A_DARK_DISCOMFORT_RENDERSYSTEM_H
