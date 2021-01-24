#ifndef A_DARK_DISCOMFORT_PLAYERSERIALISER_H
#define A_DARK_DISCOMFORT_PLAYERSERIALISER_H

#include "EntitySerialiser.h"

class PlayerSerialiser : public EntitySerialiser
{
public:

    PlayerSerialiser() :
    EntitySerialiser("Player")
    {};

    std::string Serialise(Entity* entity) override;

    Entity* Deserialise(const std::vector<std::string>& entityArgs) override;
};


#endif //A_DARK_DISCOMFORT_PLAYERSERIALISER_H
