#ifndef A_DARK_DISCOMFORT_PLAYER_H
#define A_DARK_DISCOMFORT_PLAYER_H

#include <entity/Entity.h>
#include <resource/ModelData.h>
#include <resource/Modelable.h>

class Player : public Entity, public Modelable
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
        velocity(Vec3::Zero),
        modelData(ModelData(
                "assets/models/cube/cube.obj",
                "assets/models/cube/cube.png"))
    {};

    // Public overrides

    Entity* Clone() const override;

    BoundedBox GetBoundingBox() const override;

    const ModelData& GetModelData() override;

    void SetModelData(const ModelData& modelData) override;

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

private:

    // Private fields

    float speed;

    Vec3 velocity;

    ModelData modelData;

};

#endif //A_DARK_DISCOMFORT_PLAYER_H
