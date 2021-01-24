#ifndef A_DARK_DISCOMFORT_SPLASHSCREENSERIALISER_H
#define A_DARK_DISCOMFORT_SPLASHSCREENSERIALISER_H

#include "EntitySerialiser.h"

class SplashScreenSerialiser : public EntitySerialiser
{
public:

    SplashScreenSerialiser() :
    EntitySerialiser("SplashScreen")
    {};

    std::string Serialise(Entity* entity) override;

    Entity* Deserialise(const std::vector<std::string>& entityArgs) override;
};


#endif //A_DARK_DISCOMFORT_SPLASHSCREENSERIALISER_H
