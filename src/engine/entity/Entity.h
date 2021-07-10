#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include "IndexAllocator.h"
#include <Vector3.hpp>
#include "string"
#include "EntityPtr.h"

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
            Entity(name, raylib::Vector3::Zero(), 0.f)
    {};

    /**
     * Delegate constructor for Entity, initialises
     * generational index to zero
     * @param name - a const reference to the name of the
     *               entity as a string
     * @param position - the initial position of the entity
     * @param rotation - the initial rotation of the entity
     * @param zIndex - the initial z-index of the entity,
     *                 defaults to zero
     */
    Entity(const std::string& name, raylib::Vector3 position, float rotation, int zIndex = 0) :
            position(position),
            rotation(rotation),
            name(name),
            index(GenerationalIndex()),
            zIndex(zIndex)
    {};

    virtual ~Entity() = default;

    // Virtual methods

    /**
     * A virtual update method to be overridden for entity
     * logic (runs before drawing)
     */
    virtual void OnUpdate() {};

    /**
    * A virtual update method to be overridden for entity
    * drawing instructions (runs after game logic)
    */
    virtual void OnDraw() {};

    /**
     * A virtual update method to be overridden for 2D
     * drawing instructions
     */
    virtual void OnDraw2D() {};

    /**
     * Queues the entity to be freed from storage safely at
     * the end of a frame
     */
    virtual void QueueFree();

    /**
     * A virtual method for implementing a definition of an
     * entity's BoundingBox attribute
     * @return the entity's BoundingBox
     * @note Calling this function on an object that does not
     *       override it will return a zero sized BoundingBox
     */
    virtual BoundingBox GetBoundingBox() const;

    /**
     * A virtual method to be overridden for more complex
     * object copying logic
     * @return a pointer to the cloned entity object
     */
    virtual Entity* Clone() const;

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
     *         position a Vector3
     */
    const raylib::Vector3& GetPosition() const;

    /**
     * Getter method for the entity's rotation attribute
     * @return the entity's rotation as a float
     */
    float GetRotation() const;

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
     * @param newPosition - a Vector3 to set as the
     *                      entity's position
     */
    void SetPosition(raylib::Vector3 newPosition);

    /**
     * Setter method for the entity's rotation attribute
     * @param newRotation - a float to set as the
     *                      entity's rotation
     * @note While negative rotations are accepted,
     *       rotations beyond 360 degrees are
     *       automatically wrapped
     */
    void SetRotation(float newRotation);

    /**
     * Setter method for the entity's z-index value
     * @param idx - the index value to set
     */
    void SetZIndex(int idx);

protected:

    // Protected fields

    /**
     * The position of the entity as a Vector3
     */
    raylib::Vector3 position;

    /**
     * The rotation of the entity in degrees
     */
    float rotation;

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
