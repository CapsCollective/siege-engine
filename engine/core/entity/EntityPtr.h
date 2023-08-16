//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_ENTITYPTR_H
#define SIEGE_ENGINE_ENTITYPTR_H

#include "../Statics.h"
#include "Entity.h"
#include "EntitySystem.h"

namespace Siege
{
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
    explicit EntityPtr(E* pointer) : pointer(pointer)
    {
        static_assert(std::is_base_of_v<Entity, E>);
        InitialiseIndex();
    }

    // Operator overloads

    EntityPtr& operator=(EntityPtr* rhs)
    {
        if (&rhs != this) Reset(rhs->Get());
        return *this;
    }

    EntityPtr& operator=(E* rhs)
    {
        if (rhs != pointer) Reset(rhs);
        return *this;
    }

    operator bool() const
    {
        return pointer && EntitySystem::IsLive(pointer);
    }

    E* operator->() const
    {
        return pointer;
    }

    bool operator==(Entity* rhs) const
    {
        return rhs == pointer;
    }

    bool operator<(EntityPtr<Entity> rhs) const
    {
        return pointer < rhs.pointer;
    }

    // Public methods

    /**
     * Gets the internal entity pointer as its specified
     * template type
     * @return the internal pointer to the entity
     */
    E* Get() const
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
    T* Get() const
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

    /**
     * Attempts to initialise the stored generational index
     * based on the current entity pointer
     */
    void InitialiseIndex()
    {
        if (pointer) index = pointer->GetIndex();
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
} // namespace Siege

#endif // SIEGE_ENGINE_ENTITYPTR_H
