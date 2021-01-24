#ifndef A_DARK_DISCOMFORT_GEOMETRYSERIALISER_H
#define A_DARK_DISCOMFORT_GEOMETRYSERIALISER_H

#include "EntitySerialiser.h"

class GeometrySerialiser : public EntitySerialiser
{
public:

    GeometrySerialiser() :
    EntitySerialiser("Geometry")
    {};

    std::string Serialise(Entity* entity) override;

    Entity* Deserialise(const std::vector<std::string>& entityArgs) override;
};


#endif //A_DARK_DISCOMFORT_GEOMETRYSERIALISER_H
