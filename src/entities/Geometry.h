#ifndef A_DARK_DISCOMFORT_GEOMETRY_H
#define A_DARK_DISCOMFORT_GEOMETRY_H

#include <entity/Entity.h>
#include <resource/ModelData.h>
#include <resource/Modelable.h>
#include <collision/Collidable.h>
#include <collision/CollisionSystem.h>

class Geometry : public Entity, public Modelable
{
public:

    // Public constants

    static const std::string ENTITY_NAME;

    // 'Structors

    Geometry() :
        Geometry(Vec3::Zero, 0.f, Vec3::One)
    {};

    Geometry(Vec3 position, float rotation, Vec3 dimensions) :
        Geometry(position, rotation, dimensions, ModelData(
                "assets/models/cube/cube.obj",
                "assets/models/cube/cube.png"))
    {};

    Geometry(Vec3 position, float rotation, Vec3 dimensions, const ModelData& data) :
        Entity(ENTITY_NAME, position, rotation),
        dimensions(dimensions),
        modelData(data)
    {
        // Register the entity as collidable
        CollisionSystem::Add(this);
    };

    // Public overrides

    Entity* Clone() const override;

    void QueueFree() override;

    BoundedBox GetBoundingBox() const override;

    const ModelData& GetModelData() override;

    void SetModelData(const ModelData& modelData) override;

    // Public methods

    const Vec3& GetDimensions();

protected:

    // Protected overrides

    void OnDraw() override;

private:

    // Private fields

    Vec3 dimensions;

    ModelData modelData;

};

#endif //A_DARK_DISCOMFORT_GEOMETRY_H
