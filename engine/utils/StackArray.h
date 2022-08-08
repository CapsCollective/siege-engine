//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_STACKARRAY_H
#define SIEGE_ENGINE_STACKARRAY_H

#include <cstdint>
#include <initializer_list>
#include <utility>
#include <algorithm>

namespace Siege::Utils
{
/**
 * The Array (StackArray) is a simple wrapper around C arrays which adds some convenience (such
 * as iterators and size tracking). The intention of this array is to 'replace' the standard C
 * array within the codebase in places where it makes the most sense.
 *
 * This array class does NOT guarantee that data being retrieved is valid, and it is possible to
 * retrieve garbage data. For a managed array type, please refer to the HeapArray or the MArray
 * classes.
 * @tparam T the type of the data being stored by the array.
 * @tparam S the size of the array.
 */
template<typename T, size_t S>
struct SArray
{
public:
    /**
     * The iterator class for the SArray.
     */
    struct Iterator
    {
        /**
         * The constructor for the StackArray iterator.
         * @param valuePointer a pointer to location of memory which the iterator is pointing to.
         */
        explicit Iterator(T* valuePointer) : pointer{valuePointer} {}

        /**
         * @brief The iterator prefix increment operator. This operator will increment the pointer
         * as many times as needed to find an active element. As such, it will not return unactive
         * array elements.
         * @return the iterator with the pointer and index incremented.
         */
        inline Iterator& operator++()
        {
            pointer++;
            return *this;
        }

        /**
         * @brief The postfix increment operator. Does the same thing as the postfix operator, but
         * applies it after returning the object.
         * @return the iterator before it was incremented.
         */
        inline const Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++(*this);
            return iterator;
        }

        /**
         * @brief The dereference operator.
         * @return the de-referenced pointer value.
         */
        inline T& operator*()
        {
            return *pointer;
        }
        /**
         * @brief An equal operator, checks with equality.
         * @param other the other iterator to compare to.
         * @return `true` if the pointers stored by both iterators are the same.
         */
        inline bool operator==(const Iterator& other) const
        {
            return pointer == other.pointer;
        }

        /**
         * @brief A non equality operator. Checks that twio iterators are not equal.
         * @param other the iterator to compare to.
         * @return `true` if the pointer addresses stored by both iterators are not the same.
         */
        inline bool operator!=(const Iterator& other) const
        {
            return pointer != other.pointer;
        }

        T* pointer;
    };

    /**
     * A const overload to the subscript operator. Retrieves the element stored in the provided index.
     * @param index the index to get the value from.
     * @return the value stored in the provided index.
     */
    constexpr inline const T& operator[](const size_t& index) const { return Get(index); }
    /**
     * An overload to the subscript operator. Retrieves the element stored in the provided index.
     * @param index the index to get the value from.
     * @return the value stored in the provided index.
     */
    constexpr inline T& operator[](const size_t& index) { return Get(index); }

    /**
     * Checks if a given index is in bounds of the array.
     * @param index the index to check.
     * @return true if the index is in bounds, false if the index is not in bounds.
     */
    constexpr inline bool IsInBounds(const size_t& index) { return index < S; }

    /**
     * Checks if a given index is in bounds of the array.
     * @param index the index to check.
     * @return true if the index is in bounds, false if the index is not in bounds.
     */
    constexpr inline const bool IsInBounds(const size_t& index) const { return index < S; }

    /**
     * Retrieves the element stored in the provided index as a const reference.
     * @param index the index to get the value from.
     * @return the value stored in the provided index.
     */
    constexpr inline const T& Get(const size_t& index) const
    {
        assert(IsInBounds(index) && "Provided index is out of bounds!");
        return data[index];
    }

    /**
     * Retrieves the element stored in the provided index.
     * @param index the index to get the value from.
     * @return the value stored in the provided index.
     */
    constexpr inline T& Get(const size_t& index)
    {
        assert(IsInBounds(index) && "Provided index is out of bounds!");
        return data[index];
    }

    /**
     * Inserts a value into a positon in the array.
     * @param index the index to get the value from.
     * @param element the data to insert into the array.
     */
    constexpr inline void Insert(const size_t& index, const T& element)
    {
        assert(IsInBounds(index) && "Provided index is out of bounds!");
        data[index] = element;
    }

    /**
     * Returns the static capacity of the array.
     * @return the static capacity of the array.
     */
    constexpr inline const size_t Size() const { return S; }

    /**
     * Returns the static capacity of the array.
     * @return the static capacity of the array.
     */
    constexpr inline size_t Size() { return S; }

    /**
     * Returns the raw array value as a const value.
     * @return the raw array value.
     */
    constexpr inline const T* Data() const { return data; }
    /**
     * Returns the raw array value.
     * @return the raw array value.
     */
    constexpr inline T* Data() { return data; }

    /**
     * Returns an iterator pointing to the start of the array.
     * @return an iterator pointing to the start of the array.
     */
    constexpr inline Iterator begin() { return Iterator(data); }
    /**
     * Returns an iterator pointing to the end of the array.
     * @return an iterator pointing to the end of the array.
     */
    constexpr inline Iterator end() { return Iterator(data+S); }

    /**
     * Returns an iterator pointing to the end of the array.
     * @return an iterator pointing to the end of the array.
     */
    constexpr inline const Iterator begin() const { return Iterator(data); }
    /**
     * Returns a const iterator pointing to the end of the array.
     * @return an iterator pointing to the end of the array.
     */
    constexpr inline const Iterator end() const { return Iterator(data+S); }

    T data[S];
};
}

#endif // SIEGE_ENGINE_STACKARRAY_H
