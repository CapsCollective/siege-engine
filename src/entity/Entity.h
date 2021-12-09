#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include "IndexAllocator.h"
#include "string"
#include "EntityPtr.h"
#include "../utils/Maths.h"

/**
 * The base entity class for game objects and tool
 */
class Entity
{
public:

    // Public constants

    static const std::string ENTITY_NAME;

    // 'Structors

    /**
     * Zero-param constructor for Entity, initialises both
     * position and rotation to zero, and name to "Entity"
     */
    Entity() :
        Entity(ENTITY_NAME)
    {};

    /**
     * Single-param constructor for Entity that simply defines
     * a custom name for the Entity
     * @param name - a const reference to the name of the
     *               entity as a string
     */
    Entity(const std::string& name) :
        Entity(name, {Vec3::Zero, 0.f})
    {};

    /**
     * Delegate constructor for Entity, initialises
     * generational index to zero
     * @param name - a const reference to the name of the
     *               entity as a string
     * @param transform - the initial transition of the entity
     * @param zIndex - the initial z-index of the entity,
     *                 defaults to zero
     */
    Entity(const std::string& name, const Xform& transform, int zIndex = 0);

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
     * Getter method for the entity's vanity name
     * @return a constant reference to the entity's name
     *         as a string
     */
    const std::string& GetName() const;

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
     * @return the entity's rotation as a float
     */
    float GetRotation() const;

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
     *          the EntityStorage
     */
    void SetIndex(GenerationalIndex idx);

    /**
     * Setter method for the entity's position attribute
     * @param position - a Vector3 to set as the entity's
     *                   position
     */
    void SetPosition(const Vec3& position);

    /**
     * Setter method for the entity's rotation attribute
     * @param rotation - a float to set as the entity's
     *                   rotation
     * @note While negative rotations are accepted,
     *       rotations beyond 360 degrees are
     *       automatically wrapped
     */
    void SetRotation(float rotation);

    /**
     * Setter method for the entity's scale attribute
     * @param scale - a Vector3 to set as the entity's
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
     * The transform of the entity as an Xform
     */
    Xform transform;

private:

    // Private fields

    /**
     * The name of the entity type
     */
    const std::string& name;

    /**
     * The generational index of the entity
     */
    GenerationalIndex index;

    /**
     * The entity z-index for render order
     */
    int zIndex;
};

#endif //A_DARK_DISCOMFORT_ENTITY_H
