//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_ITERATORS_H
#define SIEGE_ENGINE_ITERATORS_H

#include <cstddef>
namespace Siege
{
/**
 * A standard linear iterator. This iterator always iterates over every element of the array
 * regardless of whether it has been filled or not. This is the 'standard' iterator type
 * @tparam T the type of array stored by the iterator. Must be an array type which exposes a
 * 'Data()' and `Size()` function
 * @tparam R The type returned by the iterator (usually the type of the array)
 */
template<typename A, typename T>
class Iter
{
public:

    typedef void (Iter::*BoolType)() const;

    // TODO(Aryeh): Add more operators as needed (--, ->, etc).
    /**
     * @brief CreateIterator constructor
     * @param arrPtr the pointer to the Array
     */
    inline constexpr Iter(A* arrPtr)
    {
        ptr = arrPtr->Data() && arrPtr->Size() > 0 ? arrPtr : nullptr;
    }

    /**
     * @brief The iterator prefix increment operator. This operator will increment the pointer
     * as many times as needed to find an active element. As such, it will not return inactive
     * array elements
     * @return the iterator with the pointer and index incremented
     */
    inline constexpr Iter& operator++()
    {
        // TODO: Profile if using a reinterpret cast would be faster here
        ptr = ++index < ptr->Size() ? ptr : 0;
        return *this;
    }

    /**
     * @brief The dereference operator
     * @return the de-referenced pointer value
     */
    inline constexpr T& operator*() const
    {
        return *(ptr->Data() + index);
    }

    /**
     * Checks if the CreateIterator if valid or not
     * @return true of the CreateIterator is valid, false otherwise
     */
    inline constexpr operator BoolType() const
    {
        return ptr ? &Iter::DoNothing : 0;
    }

    /**
     * @brief An equal operator, checks with equality
     * @param other the other iterator to compare to
     * @return `true` if the pointers stored by both iterators are the same
     */
    inline constexpr bool operator==(const Iter& other) const
    {
        return ptr->Data() == other.ptr->Data() && index == other.index;
    }

    /**
     * @brief A non equality operator. Checks that two iterators are not equal
     * @param other the iterator to compare to
     * @return `true` if the pointer addresses stored by both iterators are not the same
     */
    inline constexpr bool operator!=(const Iter& other) const
    {
        return ptr->Data() != other.ptr->Data() && index != other.index;
    }

    /**
     * A de-referencing operator. Accesses a pointer to the current element
     * @return a pointer to the element pointed to by the iterator
     */
    inline constexpr T* operator->() const
    {
        return ptr->Data() + index;
    }

    /**
     * Returns the index currently held by the iterator
     * @return returns the current index the iterator is on
     */
    inline constexpr size_t GetIndex() const
    {
        return index;
    }

private:

    /**
     * A private function which does nothing (used for implementing the safe bool idiom)
     */
    inline constexpr void DoNothing() const {};

    size_t index {0};
    A* ptr {nullptr};
};

/**
 * A standard linear iterator. This iterator always iterates over every element of the array
 * regardless of whether it has been filled or not. This is the 'standard' iterator type
 * @tparam T the type of array stored by the iterator. Must be an array type which exposes a
 * 'Data()' and `Size()` function
 * @tparam R The type returned by the iterator (usually the type of the array)
 */
template<typename A, typename T>
class ConstIter
{
public:

    typedef void (ConstIter::*BoolType)() const;

    // TODO(Aryeh): Add more operators as needed (--, ->, etc).
    /**
     * @brief CreateIterator constructor
     * @param arrPtr the pointer to the Array
     */
    inline constexpr ConstIter(const A* arrPtr)
    {
        ptr = arrPtr->Data() && arrPtr->Size() > 0 ? arrPtr : nullptr;
    }

    /**
     * @brief The iterator prefix increment operator. This operator will increment the pointer
     * as many times as needed to find an active element. As such, it will not return inactive
     * array elements
     * @return the iterator with the pointer and index incremented
     */
    inline constexpr ConstIter& operator++()
    {
        // TODO: Profile if using a reinterpret cast would be faster here
        ptr = ++index < ptr->Size() ? ptr : 0;
        return *this;
    }

    /**
     * @brief The dereference operator
     * @return the de-referenced pointer value
     */
    inline constexpr const T& operator*() const
    {
        return *(ptr->Data() + index);
    }

    /**
     * Checks if the CreateIterator if valid or not
     * @return true of the CreateIterator is valid, false otherwise
     */
    inline constexpr operator BoolType() const
    {
        return ptr ? &ConstIter::DoNothing : 0;
    }

    /**
     * @brief An equal operator, checks with equality
     * @param other the other iterator to compare to
     * @return `true` if the pointers stored by both iterators are the same
     */
    inline constexpr bool operator==(const ConstIter& other) const
    {
        return ptr->Data() == other.ptr->Data() && index == other.index;
    }

    /**
     * @brief A non equality operator. Checks that two iterators are not equal
     * @param other the iterator to compare to
     * @return `true` if the pointer addresses stored by both iterators are not the same
     */
    inline constexpr bool operator!=(const ConstIter& other) const
    {
        return ptr->Data() != other.ptr->Data() && index != other.index;
    }

    /**
     * A de-referencing operator. Accesses a pointer to the current element
     * @return a pointer to the element pointed to by the iterator
     */
    inline constexpr const T* operator->() const
    {
        return ptr->Data() + index;
    }

    /**
     * Returns the index currently held by the iterator
     * @return returns the current index the iterator is on
     */
    inline constexpr size_t GetIndex() const
    {
        return index;
    }

private:

    /**
     * A private function which does nothing (used for implementing the safe bool idiom)
     */
    inline constexpr void DoNothing() const {};

    size_t index {0};
    const A* ptr {nullptr};
};

/**
 * A 'Managed' iterator. This iterator assumes that an array has some way of checking if an index
 * has been assigned (via an 'Active()' class).
 * @tparam T the type of array stored by the iterator. Must be an array type which exposes a
 * 'Data()', `Size()`, and 'Active()' function
 * @tparam R The type returned by the iterator (usually the type of the array)
 */
template<typename A, typename T>
class MIter
{
public:

    typedef void (MIter::*BoolType)() const;

    // TODO(Aryeh): Add more operators as needed (--, ->, etc).
    /**
     * @brief CreateIterator constructor. Automatically sets the iterator to the first active
     * element
     * @param arrPtr the pointer to the array
     */
    inline constexpr MIter(A* arrPtr) : ptr {arrPtr}
    {
        index = arrPtr->GetFirstElementIdx();
        ptr = arrPtr->Data() && arrPtr->Count() > 0 ? arrPtr : nullptr;

        while (ptr && !ptr->IsActive(index)) index++;
    }

    /**
     * @brief The iterator prefix increment operator. This operator will increment the pointer
     * as many times as needed to find an active element. As such, it will not return inactive
     * array elements
     * @return the iterator with the pointer and index incremented
     */
    inline constexpr MIter& operator++()
    {
        // If the next element in the array is invalid, keep incrementing until we find one that
        // is
        while (ptr->Data() && !ptr->IsActive(index + 1)) index++;

        // TODO: Profile if using a reinterpret cast would be faster here
        ptr = index < ptr->Size() ? ptr : 0;

        index++;

        return *this;
    }

    /**
     * @brief The dereference operator
     * @return the de-referenced pointer value
     */
    inline constexpr T& operator*() const
    {
        return *(ptr->Data() + index);
    }

    /**
     * Checks if the CreateIterator if valid or not
     * @return true of the CreateIterator is valid, false otherwise
     */
    inline constexpr operator BoolType() const
    {
        return ptr ? &MIter::DoNothing : 0;
    }

    /**
     * @brief An equal operator, checks with equality
     * @param other the other iterator to compare to
     * @return `true` if the pointers stored by both iterators are the same
     */
    inline constexpr bool operator==(const MIter& other) const
    {
        return ptr->Data() == other.ptr->Data() && index == other.index;
    }

    /**
     * @brief A non equality operator. Checks that two iterators are not equal
     * @param other the iterator to compare to
     * @return `true` if the pointer addresses stored by both iterators are not the same
     */
    inline constexpr bool operator!=(const MIter& other) const
    {
        return ptr->Data() != other.ptr->Data() && index != other.index;
    }

    /**
     * A de-referencing operator. Accesses a pointer to the current element
     * @return a pointer to the element pointed to by the iterator
     */
    inline constexpr T* operator->() const
    {
        return ptr->Data() + index;
    }

    /**
     * Returns the index currently held by the iterator
     * @return returns the current index the iterator is on
     */
    inline constexpr size_t GetIndex() const
    {
        return index;
    }

private:

    /**
     * A private function which does nothing (used for implementing the safe bool idiom)
     */
    inline constexpr void DoNothing() const {};

    // private variables

    size_t index {0};
    A* ptr {nullptr};
};

/**
 * A 'Managed' iterator. This iterator assumes that an array has some way of checking if an index
 * has been assigned (via an 'Active()' class).
 * @tparam T the type of array stored by the iterator. Must be an array type which exposes a
 * 'Data()', `Size()`, and 'Active()' function
 * @tparam R The type returned by the iterator (usually the type of the array)
 */
template<typename A, typename T>
class CMIter
{
public:

    typedef void (CMIter::*BoolType)() const;

    // TODO(Aryeh): Add more operators as needed (--, ->, etc).
    /**
     * @brief CreateIterator constructor. Automatically sets the iterator to the first active
     * element
     * @param arrPtr the pointer to the array
     */
    inline constexpr CMIter(const A* arrPtr) : ptr {arrPtr}
    {
        index = arrPtr->GetFirstElementIdx();
        ptr = arrPtr->Data() && arrPtr->Count() > 0 ? arrPtr : nullptr;

        while (ptr && !ptr->IsActive(index)) index++;
    }

    /**
     * @brief The iterator prefix increment operator. This operator will increment the pointer
     * as many times as needed to find an active element. As such, it will not return inactive
     * array elements
     * @return the iterator with the pointer and index incremented
     */
    inline constexpr CMIter& operator++()
    {
        // If the next element in the array is invalid, keep incrementing until we find one that
        // is
        while (ptr->Data() && !ptr->IsActive(index + 1)) index++;

        // TODO: Profile if using a reinterpret cast would be faster here
        ptr = index < ptr->Size() ? ptr : 0;

        index++;

        return *this;
    }

    /**
     * @brief The dereference operator
     * @return the de-referenced pointer value
     */
    inline constexpr const T& operator*() const
    {
        return *(ptr->Data() + index);
    }

    /**
     * Checks if the CreateIterator if valid or not
     * @return true of the CreateIterator is valid, false otherwise
     */
    inline constexpr operator BoolType() const
    {
        return ptr ? &CMIter::DoNothing : 0;
    }

    /**
     * @brief An equal operator, checks with equality
     * @param other the other iterator to compare to
     * @return `true` if the pointers stored by both iterators are the same
     */
    inline constexpr bool operator==(const CMIter& other) const
    {
        return ptr->Data() == other.ptr->Data() && index == other.index;
    }

    /**
     * @brief A non equality operator. Checks that two iterators are not equal
     * @param other the iterator to compare to
     * @return `true` if the pointer addresses stored by both iterators are not the same
     */
    inline constexpr bool operator!=(const CMIter& other) const
    {
        return ptr->Data() != other.ptr->Data() && index != other.index;
    }

    /**
     * A de-referencing operator. Accesses a pointer to the current element
     * @return a pointer to the element pointed to by the iterator
     */
    inline constexpr const T* operator->() const
    {
        return ptr->Data() + index;
    }

    /**
     * Returns the index currently held by the iterator
     * @return returns the current index the iterator is on
     */
    inline constexpr size_t GetIndex() const
    {
        return index;
    }

private:

    /**
     * A private function which does nothing (used for implementing the safe bool idiom)
     */
    inline constexpr void DoNothing() const {};

    // private variables

    size_t index {0};
    const A* ptr {nullptr};
};
} // namespace Siege

#endif // SIEGE_ENGINE_ITERATORS_H
