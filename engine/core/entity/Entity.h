//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_ENTITY_H
#define SIEGE_ENGINE_ENTITY_H

#include <utils/String.h>
#include <utils/Token.h>
#include <utils/math/Maths.h>
#include <utils/math/Xform.h>

#include "EntityPtr.h"
#include "IndexAllocator.h"

REGISTER_TOKEN(Entity);

namespace Siege
{
/**
 * The base entity class for game objects and tool
 */
class Entity
{
public:

    // 'Structors

    /**
     * Zero-param constructor for Entity, initialises both
     * position and rotation to zero, and name to "Entity"
     */
    Entity() : Entity(TOKEN_Entity) {};

    /**
     * Single-param constructor for Entity that simply defines
     * a custom name for the Entity
     * @param typeName - a const reference to the name of the
     *                   entity as a string
     */
    Entity(Token type) : Entity(type, {Vec3::Zero(), 0.f}) {};

    /**
     * Delegate constructor for Entity, initialises
     * generational index to zero
     * @param name - a const reference to the name of the
     *               entity as a string
     * @param transform - the initial transition of the entity
     * @param zIndex - the initial z-index of the entity,
     *                 defaults to zero
     */
    Entity(Token type, const Xform& transform, int zIndex = 0);

    /**
     * Default virtual destructor for Entity
     */
    virtual ~Entity() = default;

    // Virtual methods

    /**
     * A virtual start method to be overridden for entity
     * logic (runs before update)
     */
    virtual void OnStart() {};

    /**
     * A virtual update method to be overridden for entity
     * logic (runs before drawing)
     */
    virtual void OnUpdate() {};

    /**
     * A virtual destroy method to be overridden for entity
     * logic (runs before freeing)
     */
    virtual void OnDestroy() {};

    /**
     * A virtual update method to be overridden for 2D
     * drawing instructions
     */
    virtual void OnDraw2D() {};

    /**
     * A virtual update method to be overridden for 3D
     * drawing instructions
     */
    virtual void OnDraw3D() {};

    /**
     * A virtual method for implementing a definition of an
     * entity's BoundedBox attribute
     * @return the entity's BoundedBox
     * @note Calling this function on an object that does not
     *       override it will return a zero sized BoundedBox
     */
    virtual BoundedBox GetBoundingBox() const;

    /**
     * A virtual method to be overridden for more complex
     * object copying logic
     * @return a pointer to the cloned entity object
     */
    virtual Entity* Clone() const;

    // Public methods

    /**
     * Queues the entity to be freed from storage safely at
     * the end of a frame
     */
    void QueueFree();

    // Public getters

    /**
     * Getter method for the entity's type
     * @return a token for the entity's type
     */
    Token GetType() const;

    /**
     * Getter method for the entity's generational index
     * @return a constant reference to the entity's
     *         GenerationalIndex
     */
    const GenerationalIndex& GetIndex() const;

    /**
     * Getter method for the entity's position attribute
     * @return a constant reference to the entity's
     *         position as a Vec3
     */
    const Vec3& GetPosition() const;

    /**
     * Getter method for the entity's rotation attribute
     * @return the entity's rotation as a Vec3
     */
    const Vec3& GetRotation() const;

    /**
     * Getter method for the entity's scale attribute
     * @return the entity's scale as a Vec3
     */
    const Vec3& GetScale() const;

    /**
     * Getter method for the entity's transform
     * @return a constant reference to the entity's
     *         transform values
     */
    const Xform& GetTransform() const;

    /**
     * Getter method for the entity's transform
     * @return a reference to the entity's
     *         transform values
     */
    Xform& GetTransform();

    /**
     * Getter method for the entity's z-index attribute
     * @return the entity's z-index as an int
     */
    int GetZIndex() const;

    // Public setters

    /**
     * Setter method for the entity's generational index
     * @param idx - a GenerationalIndex to set as the
     *              entity's generational index
     * @warning This method should really only be used by
     *          the EntitySystem
     */
    void SetIndex(GenerationalIndex idx);

    /**
     * Setter method for the entity's transform
     * @param xform - a transform to set to the entity
     */
    void SetTransform(const Xform& xform);

    /**
     * Setter method for the entity's position attribute
     * @param position - a Vec3 to set as the entity's
     *                   position
     */
    void SetPosition(const Vec3& position);

    /**
     * Setter method for the entity's rotation attribute
     * @param rotation - a Vec3 to set as the entity's
     *                   rotation
     * @note While negative rotations are accepted,
     *       rotations beyond 360 degrees are
     *       automatically wrapped
     */
    void SetRotation(const Vec3& rotation);

    /**
     * Setter method for the entity's scale attribute
     * @param scale - a Vec3 to set as the entity's
     *                scale
     */
    void SetScale(const Vec3& scale);

    /**
     * Setter method for the entity's z-index value
     * @param idx - the index value to set
     */
    void SetZIndex(int idx);

protected:

    // Protected fields

    /**
     * The transform of the entity as a Xform
     */
    Xform transform;

private:

    // Private fields

    /**
     * The type of the entity type
     */
    Token type;

    /**
     * The generational index of the entity
     */
    GenerationalIndex index;

    /**
     * The entity z-index for render order
     */
    int zIndex;
};
} // namespace Siege

#endif // SIEGE_ENGINE_ENTITY_H
