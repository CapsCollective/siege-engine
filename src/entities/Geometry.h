#ifndef A_DARK_DISCOMFORT_GEOMETRY_H
#define A_DARK_DISCOMFORT_GEOMETRY_H

#include <entity/Entity.h>
#include <collision/Collidable.h>
#include <collision/CollisionSystem.h>
#include <render/RenderSystem.h>

#include <utility>

class Geometry : public Entity
{
public:

    // Public constants

    static const std::string ENTITY_NAME;

    // 'Structors

    Geometry() :
        Geometry({Vec3::Zero, 0.f})
    {};

    explicit Geometry(const Xform& transform) :
        Geometry(transform,"assets/models/cube/cube.obj","assets/models/cube/cube.png")
    {};

    Geometry(const Xform& transform, std::string modelPath, std::string texturePath) :
        Entity(ENTITY_NAME, transform),
        modelPath(std::move(modelPath)),
        texturePath(std::move(texturePath))
    {};

    // Public overrides

    Entity* Clone() const override;

    BoundedBox GetBoundingBox() const override;

    ModelData GetModelData();

    // Public methods

    const Vec3& GetDimensions();

protected:

    // Protected overrides

    void OnStart() override;

    void OnDestroy() override;

private:

    // Private fields

    std::string modelPath, texturePath;

};

#endif //A_DARK_DISCOMFORT_GEOMETRY_H
