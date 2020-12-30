#ifndef A_DARK_DISCOMFORT_GEOMETRY_H
#define A_DARK_DISCOMFORT_GEOMETRY_H

#include "Entity.h"
#include "../resources/ModelData.h"

class Geometry : public Entity
{
public:

    // Constructors

    Geometry() :
    Geometry(raylib::Vector3::Zero(), 0.f, raylib::Vector3::One())
    {};

    explicit Geometry(raylib::Vector3 position, float rotation, raylib::Vector3 dimensions) :
    Geometry(position, rotation, dimensions, ModelData("assets/models/cube/cube.obj",
                                                       "assets/models/cube/CubeTexture.png"))
    {};

    explicit Geometry(raylib::Vector3 position, float rotation, raylib::Vector3 dimensions, const ModelData& data) :
    Entity(position, rotation),
    dimensions(dimensions),
    modelData(data)
    {
        Entity::SetName("Geometry");
    };

    // Public overrides

    Entity* Clone() override;

    BoundingBox GetBoundingBox() override;

    // Public methods

    raylib::Vector3 GetDimensions();

    ModelData& GetModelData();

protected:

    // Protected overrides

    void OnDraw() override;

private:

    // Private fields

    raylib::Vector3 dimensions;

    ModelData modelData;
};

#endif //A_DARK_DISCOMFORT_GEOMETRY_H
