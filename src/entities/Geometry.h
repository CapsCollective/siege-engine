//
// Created by Jonathan Moallem on 10/12/20.
//

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
    {};

    // Public overrides

    BoundingBox GetBoundingBox() override;

protected:

    // Protected overrides

    void OnDraw() override;

    void QueueFree() override;

private:

    // Private fields

    raylib::Vector3 dimensions;
};


#endif //A_DARK_DISCOMFORT_GEOMETRY_H
