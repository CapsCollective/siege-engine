#ifndef A_DARK_DISCOMFORT_ENTITY3D_H
#define A_DARK_DISCOMFORT_ENTITY3D_H

#include "Entity.h"

class Entity3D : public Entity
{
public:

    // Virtual methods

    /**
    * A virtual update method to be overridden for entity
    * drawing instructions (runs after game logic)
    */
    virtual void OnDraw() {};

    /**
     * A virtual method for implementing a definition of an
     * entity's BoundingBox attribute
     * @return The entity's BoundingBox
     * @note Calling this function on an object that does not
     *       override it will return a zero sized BoundingBox
     */
    virtual BoundingBox GetBoundingBox();

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

protected:

    // Constructors

    /**
     * Zero-param constructor for Entity, initialises both
     * position and rotation to zero
     */
    Entity3D() :
    Entity3D(raylib::Vector3::Zero(), 0.f)
    {};

    /**
     * Delegate constructor for Entity, initialises
     * generational index to zero and name to "Entity"
     * @param position - the initial position of the entity
     * @param rotation - the initial rotation of the entity
     */
    Entity3D(raylib::Vector3 position, float rotation) :
    rotation(rotation),
    position(position),
    Entity()
    {};

    // Protected fields

    /**
     * The rotation of the entity in degrees
     */
    float rotation;

    /**
     * The position of the entity as a Vector3
     */
    raylib::Vector3 position;
};


#endif //A_DARK_DISCOMFORT_ENTITY3D_H
