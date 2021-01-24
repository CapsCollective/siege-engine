#include "SplashScreenSerialiser.h"
#include "../../../entities/SplashScreen.h"

std::string SplashScreenSerialiser::Serialise(Entity *entity)
{
    return EntitySerialiser::Serialise(entity);
}

Entity *SplashScreenSerialiser::Deserialise(const std::vector<std::string>& entityArgs)
{
    return new SplashScreen();
}
