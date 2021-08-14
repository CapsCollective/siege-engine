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
        Player({Vec3::Zero, 0.f})
    {};

    explicit Player(const Xform& transform) :
        Entity(ENTITY_NAME, transform),
        speed(1.5f),
        velocity(Vec3::Zero)
    {};

    // Public overrides

    Entity* Clone() const override;

    BoundedBox GetBoundingBox() const override;

protected:

    // Protected overrides

    void OnStart() override;

    void OnUpdate() override;

    void OnDestroy() override;

private:

    // Private fields

    float speed;

    Vec3 velocity;

};

#endif //A_DARK_DISCOMFORT_PLAYER_H
