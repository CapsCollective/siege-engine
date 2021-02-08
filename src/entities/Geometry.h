#ifndef A_DARK_DISCOMFORT_GEOMETRY_H
#define A_DARK_DISCOMFORT_GEOMETRY_H

#include "../systems/entity/Entity.h"
#include "../systems/resource/ModelData.h"
#include "../systems/resource/Modelable.h"
#include "../systems/collision/Collidable.h"
#include "../systems/collision/CollisionSystem.h"

class Geometry : public Entity, public Modelable
{
public:

    // Constructors

    Geometry() :
    Geometry(raylib::Vector3::Zero(), 0.f, raylib::Vector3::One())
    {};

    Geometry(raylib::Vector3 position, float rotation, raylib::Vector3 dimensions) :
    Geometry(position, rotation, dimensions, ModelData(
            "assets/models/cube/cube.obj",
            "assets/models/cube/cube.png"))
    {};

    Geometry(raylib::Vector3 position, float rotation, raylib::Vector3 dimensions, const ModelData& data) :
    Entity(position, rotation),
    dimensions(dimensions),
    modelData(data)
    {
        Entity::SetName("Geometry");

        // Register the entity as collidable
        CollisionSystem::Add(this);
    };

    // Public overrides

    Entity* Clone() override;

    void QueueFree() override;

    BoundingBox GetBoundingBox() override;

    const ModelData& GetModelData() override;

    void SetModelData(const ModelData& modelData) override;

    // Public methods

    const raylib::Vector3& GetDimensions();

protected:

    // Protected overrides

    void OnDraw() override;

private:

    // Private fields

    raylib::Vector3 dimensions;

    ModelData modelData;
};

#endif //A_DARK_DISCOMFORT_GEOMETRY_H
