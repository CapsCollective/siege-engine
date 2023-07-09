//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_GEOMETRY_H
#define SIEGE_ENGINE_GEOMETRY_H

#include <core/entity/Entity.h>
#include <core/render/RenderSystem.h>

#include <utility>

class Geometry : public Siege::Entity
{
public:

    // Public constants

    static const Siege::String ENTITY_NAME;

    // 'Structors

    Geometry() : Geometry({Siege::Vec3::Zero(), 0.f}) {};

    explicit Geometry(const Siege::Xform& transform) :
        Geometry(transform, "models/cube/cube.obj", "models/cube/cube.png") {};

    Geometry(const Siege::Xform& transform, Siege::String modelPath, Siege::String texturePath) :
        Entity(ENTITY_NAME, transform),
        modelPath(std::move(modelPath)),
        texturePath(std::move(texturePath)) {};

    // Public overrides

    Entity* Clone() const override;

    Siege::BoundedBox GetBoundingBox() const override;

    Siege::ModelData GetModelData();

    // Public methods

    const Siege::Vec3& GetDimensions();

protected:

    // Protected overrides

    void OnStart() override;

    void OnDestroy() override;

private:

    // Private fields

    Siege::String modelPath, texturePath;
};

#endif // SIEGE_ENGINE_GEOMETRY_H
