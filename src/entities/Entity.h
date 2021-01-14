#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include "../entity_system/IndexAllocator.h"
#include <raylib-cpp.hpp>

/**
 * The base entity class for game objects and tool
 */
class Entity
{
public:

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
     * A virtual update method to be overridden for entity
     * UI drawing instructions (runs after drawing)
     */
    virtual void OnUIDraw() {};

    /**
     * A virtual method for implementing more complex object
     * copying logic
     * @return A pointer to the cloned entity object
     * @warning Calling this function on an object that does
     *          not override it will return a nullptr
     * TODO potentially move into cloneable interface
     */
    virtual Entity* Clone();

    /**
     * A virtual method for implementing a definition of an
     * entity's BoundingBox attribute
     * @return The entity's BoundingBox
     * @note Calling this function on an object that does not
     *       override it will return a zero sized BoundingBox
     */
    virtual BoundingBox GetBoundingBox();

    // Public methods

    /**
     * Queues the entity to be freed from storage safely at
     * the end of a frame
     */
    void QueueFree();

    /**
     * Unsafely removes the entity from storage immediately
     * @warning This method is very unsafe and QueueFree()
     *          should be used wherever possible instead
     */
    void Free();

    // Public getters

    /**
     * Getter method for the entity's position attribute
     * @return A constant reference to the entity's
     *         position a Vector3
     */
    const raylib::Vector3& GetPosition() const;

    /**
     * Getter method for the entity's rotation attribute
     * @return The entity's rotation as a float
     */
    float GetRotation() const;

    /**
     * Getter method for the entity's vanity name
     * @return A constant reference to the entity's name
     *         as a string
     */
    const std::string& GetName() const;

    /**
     * Getter method for the entity's generational index
     * @return A constant reference to the entity's
     *         GenerationalIndex
     */
    const GenerationalIndex& GetIndex() const;

    // Public setters

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
     * Setter method for the entity's generational index
     * @param idx - a GenerationalIndex to set as the
     *              entity's generational index
     * @warning This method should really only be used by
     *          the EntityStorage
     */
    void SetIndex(GenerationalIndex idx);

protected:

    // Constructors

    /**
     * Zero-param constructor for Entity, initialises both
     * position and rotation to zero
     */
    Entity() :
    Entity(raylib::Vector3::Zero(), 0.f)
    {};

    /**
     * Delegate constructor for Entity, initialises
     * generational index to zero and name to "Entity"
     * @param position - the initial position of the entity
     * @param rotation - the initial rotation of the entity
     */
    explicit Entity(raylib::Vector3 position, float rotation) :
    rotation(rotation),
    position(position),
    index(GenerationalIndex()),
    name("Entity")
    {};

    // Protected methods

    /**
     * Setter method for the entity's vanity name
     * @param entityName - a string to set as the entity's
     *                     vanity name
     * @warning Do not use this method outside of the
     *          constructor as doing so will produce
     *          undefined behaviour during serialisation
     */
    void SetName(std::string entityName);

    // Protected fields

    /**
     * The rotation of the entity in degrees
     */
    float rotation;

    /**
     * The position of the entity as a Vector3
     */
    raylib::Vector3 position;

private:

    // Private fields

    /**
     * The name of the entity type
     */
    std::string name;

    /**
     * The generational index of the entity
     */
    GenerationalIndex index;
};

#endif //A_DARK_DISCOMFORT_ENTITY_H
