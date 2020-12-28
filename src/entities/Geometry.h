#ifndef A_DARK_DISCOMFORT_GEOMETRY_H
#define A_DARK_DISCOMFORT_GEOMETRY_H

#include "Entity.h"

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
};


#endif //A_DARK_DISCOMFORT_GEOMETRY_H
