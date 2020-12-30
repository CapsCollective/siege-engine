#ifndef A_DARK_DISCOMFORT_GEOMETRY_H
#define A_DARK_DISCOMFORT_GEOMETRY_H

#include "Entity.h"
#include "../resources/ModelData.h"

class Geometry : public Entity
{
public:

    // Constructors

    Geometry() :
    Geometry(raylib::Vector3::Zero(), raylib::Vector3::One())
    {};

    explicit Geometry(raylib::Vector3 pos, raylib::Vector3 dimensions) :
    Entity(pos),
    dimensions(dimensions)
    {
        Entity::SetName("Geometry");
    };

    explicit Geometry(raylib::Vector3 pos, raylib::Vector3 dimensions, const ModelData& model) :
    Entity(pos),
    dimensions(dimensions),
    modelData(model)
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
