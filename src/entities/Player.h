#ifndef A_DARK_DISCOMFORT_PLAYER_H
#define A_DARK_DISCOMFORT_PLAYER_H

#include "Entity.h"

class Player : public Entity
{
public:

    // Constructors

    Player() :
    Player(raylib::Vector3::Zero())
    {};

    explicit Player(raylib::Vector3 position) :
    Entity(position),
    speed(1.5f),
    velocity(raylib::Vector3::Zero())
    {
        Entity::SetName("Player");
    };

    // Public overrides

    BoundingBox GetBoundingBox() override;

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

    void OnUIDraw() override;

private:

    // Private fields

    float speed;

    raylib::Vector3 velocity;
};

#endif //A_DARK_DISCOMFORT_PLAYER_H
