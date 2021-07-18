#ifndef A_DARK_DISCOMFORT_ENTITYPTR_H
#define A_DARK_DISCOMFORT_ENTITYPTR_H

#include "Entity.h"
#include "EntityStorage.h"

template<class E>
class EntityPtr
{
public:

    // 'Structors

    /**
     * Basic entity pointer constructor that initialises
     * the entity pointer object
     * @param pointer - a pointer to an entity object
     */
    explicit EntityPtr(E* pointer) :
        pointer(pointer)
    {
        static_assert(std::is_base_of_v<Entity, E>);
        if (pointer) index = pointer->GetIndex();
    }

    // Operator overloads

    EntityPtr& operator =(EntityPtr* rhs)
    {
        if (&rhs != this) Reset(rhs->Get());
        return *this;
    }

    EntityPtr& operator =(E* rhs)
    {
        if (rhs != pointer) Reset(rhs);
        return *this;
    }

    explicit operator bool() const
    {
        return pointer && EntityStorage::IsLive(index);
    }

    E* operator ->() const
    {
        return pointer;
    }

    // Public methods

    /**
     * Gets the internal entity pointer as its specified
     * template type
     * @return the internal pointer to the entity
     */
    E* Get()
    {
        return pointer;
    }

    /**
     * Gets the internal entity pointer casted to the specified
     * template type
     * @tparam T - the type to cast the internal pointer to
     * @return the entity pointer, casted to the specified type
     */
    template<typename T>
    T* Get()
    {
        return dynamic_cast<T*>(pointer);
    }

    /**
     * Resets the entity pointer with a new entity pointer
     * @param other - the
     */
    void Reset(E* other)
    {
        pointer = other;
        index = other ? other->GetIndex() : GenerationalIndex();
    }

private:

    // Private fields

    /**
     * The pointer to the held entity
     */
    E* pointer;

    /**
     * The generational index of the held entity
     */
    GenerationalIndex index;
};

#endif //A_DARK_DISCOMFORT_ENTITYPTR_H
