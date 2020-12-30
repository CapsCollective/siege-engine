#ifndef A_DARK_DISCOMFORT_GEOMETRY_H
#define A_DARK_DISCOMFORT_GEOMETRY_H

#include "Entity.h"

class Geometry : public Entity
{
public:

    // Constructors

    Geometry() :
    Geometry(raylib::Vector3::Zero(), 0.f, raylib::Vector3::One())
    {};

    explicit Geometry(raylib::Vector3 position, float rotation, raylib::Vector3 dimensions) :
    Entity(position, rotation),
    dimensions(dimensions)
    {
        Entity::SetName("Geometry");
    };

    // Public overrides

    Entity* Clone() override;

    BoundingBox GetBoundingBox() override;

    // Public methods

    raylib::Vector3 GetDimensions();

protected:

    // Protected overrides

    void OnDraw() override;

private:

    // Private fields

    raylib::Vector3 dimensions;

    // TODO add model data
};


#endif //A_DARK_DISCOMFORT_GEOMETRY_H
