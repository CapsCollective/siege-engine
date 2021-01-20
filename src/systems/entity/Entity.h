#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include "IndexAllocator.h"
#include <Vector3.hpp>
#include "string"

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
     * Zero-param constructor for Entity, initialises all
     * fields to zero
     */
    Entity() :
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
