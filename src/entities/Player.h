#ifndef A_DARK_DISCOMFORT_PLAYER_H
#define A_DARK_DISCOMFORT_PLAYER_H

#include <entity/Entity.h>
#include <render/RenderSystem.h>

class Player : public Entity
{
public:

    // Public constants

    static const std::string ENTITY_NAME;

    // 'Structors

    Player() :
        Player(Vec3::Zero, 0.f)
    {};

    Player(Vec3 position, float rotation) :
        Entity(ENTITY_NAME, position, rotation),
        speed(1.5f),
        velocity(Vec3::Zero)
    {};

    // Public overrides

    Entity* Clone() const override;

    void QueueFree() override;

    BoundedBox GetBoundingBox() const override;

protected:

    // Protected overrides

    void OnStart() override;

    void OnUpdate() override;

private:

    // Private fields

    float speed;

    Vec3 velocity;

};

#endif //A_DARK_DISCOMFORT_PLAYER_H
