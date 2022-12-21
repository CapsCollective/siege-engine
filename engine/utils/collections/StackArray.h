//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_STACK_ARRAY_H
#define SIEGE_ENGINE_STACK_ARRAY_H

#include "ArrayUtils.h"
#include "BitSet.h"
#include "Iterators.h"

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
     * A const overload to the subscript operator. Retrieves the element stored in the provided
     * index.
     * @param index the index to get the value from.
     * @return the value stored in the provided index.
     */
    constexpr inline const T& operator[](const size_t& index) const
    {
        return Get(index);
    }
    /**
     * An overload to the subscript operator. Retrieves the element stored in the provided index.
     * @param index the index to get the value from.
     * @return the value stored in the provided index.
     */
    constexpr inline T& operator[](const size_t& index)
    {
        return Get(index);
    }

    /**
     * Checks if a given index is in bounds of the array.
     * @param index the index to check.
     * @return true if the index is in bounds, false if the index is not in bounds.
     */
    constexpr inline bool IsInBounds(const size_t& index)
    {
        return index < S;
    }

    /**
     * Checks if a given index is in bounds of the array.
     * @param index the index to check.
     * @return true if the index is in bounds, false if the index is not in bounds.
     */
    constexpr inline const bool IsInBounds(const size_t& index) const
    {
        return index < S;
    }

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
     * Inserts a value into a position in the array.
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
    constexpr inline const size_t Size() const
    {
        return S;
    }

    /**
     * Returns the static capacity of the array.
     * @return the static capacity of the array.
     */
    constexpr inline size_t Size()
    {
        return S;
    }

    /**
     * Returns the raw array value as a const value.
     * @return the raw array value.
     */
    constexpr inline const T* Data() const
    {
        return data;
    }
    /**
     * Returns the raw array value.
     * @return the raw array value.
     */
    constexpr inline T* Data()
    {
        return data;
    }

    /**
     * Creates a fast iterator type. The fast iterator iterates over every single element in an
     * array, regardless of whether they have been assigned to.
     * @return an Iter instance to iterate over the array
     */
    constexpr inline Utils::Iter<SArray<T, S>, T> Iterator()
    {
        return {this};
    }

    T data[S];
};

template<typename T, size_t S>
/**
 * @brief The Managed Stack Array (MSArray) is a managed stack-allocated array type. The MSArray
 * is intended to be used when a container is needed who's size is known at compile time, but is
 * required to be constant once a size is provided.
 *
 * The MSArray tracks all data that's been inserted and removed from the array. All data inserted
 * into the array will have its index marked as active. Accessing an index which has not been
 * activated will trigger an immediate assertion failure. As such, the HeapArray wants you to
 * only access data which has been explicitly entered into it, thus avoiding issues where 'garbage'
 * data may be accessed through.
 *
 * The MSArray is a 'safer' alternative to the StackArray(SArray), however it does sacrifice memory
 * usage for safety.
 * @tparam T the type being stored by the array.
 * @tparam S the static size of the array.
 */
class MSArray
{
public:

    /**
     * Default constructor.
     */
    MSArray() : MSArray(0) {}

    /**
     * Initializer list constructor. this constructor allows a MSArray to be constructed using a
     * list format. For example: `MSArray<int, 2> arr = {1, 2};`. If more parameters are passed
     * into the initializer list thant the static size of the array, only the number of elements
     * equal to the static size will be copied.
     * @param list the parameter list to be added to the MSArray.
     */
    MSArray(std::initializer_list<T> list) : MSArray(list.size())
    {
        count = ArrayUtils::SetCount(list.size(), 0, S);
        ArrayUtils::CopyData(data, std::data(list), sizeof(T) * count);
        bitField.SetBitsToOne(count);
    }

    /**
     * Initializes a MSArray with a raw pointer. NOTE: if the size provided is larger than the size
     * provided to MSArray, then it will copy data up to the static size of MSArray.
     * @param ptr
     * @param ptrSize
     */
    MSArray(const T* ptr, const size_t& ptrSize) : MSArray(ptrSize)
    {
        count = ArrayUtils::SetCount(ptrSize, 0, S);
        ArrayUtils::CopyData(data, ptr, sizeof(T) * count);
        bitField.SetBitsToOne(count);
    }

    /**
     * Copies two equally sized SMArrays.
     * @param other the other array to be copied.
     */
    MSArray(MSArray& other)
    {
        Copy(other);
    }

    /**
     * Moves the contents of one array to another.
     * @param other The array to be moved. The array being moved will be reset after its contents
     * have been moved.
     */
    MSArray(MSArray&& other)
    {
        Swap(other);
    }

    // Operator Overloads

    /**
     * A constant subscript operator. This operator returns the value stored in the provided index
     * as long as the index position is valid and the element is active.
     * @param index the index position to search.
     * @return a constant reference to the value stored in the index.
     */
    inline const T& operator[](const size_t& index) const
    {
        return Get(index);
    }

    /**
     * A subscript operator. This operator returns the value stored in the provided index
     * as long as the index position is valid and the element is active.
     * @param index the index position to search.
     * @return a mutable reference to the value stored in the index.
     */
    inline T& operator[](const size_t& index)
    {
        count += !bitField.IsSet(index + 1);
        bitField.SetBit(index + 1);
        return Get(index);
    }

    /**
     * A copy assignment operator for the MSArray. This operator only allows the copying of data
     * between MSArrays with the same size and type.
     * @param other the array to be copied.
     * @return a reference to the current array in which data was copied.
     */
    inline MSArray& operator=(const MSArray& other)
    {
        Copy(other);
        return *this;
    }

    /**
     * A move assignment operator for the MSArray. This operator only allows the moving of data
     * between MSArrays with the same size and type.
     * @param other the array to be moved.
     * @return a reference to the current array in which data was moved.
     */
    inline MSArray& operator=(MSArray&& other)
    {
        Copy(other);
        other.Clear();
        return *this;
    }

    // Getters

    /**
     * Gets a constant reference to a value stored in the specified index as long as the index
     * position is valid and the element is active.
     * @param index the index position to search.
     * @return a constant reference to the value stored in the index.
     */
    inline const T& Get(const size_t& index) const
    {
        BitUtils::AssertIsSet(bitField.BitField(), index);
        ArrayUtils::AssertIsInBounds(index, S);
        return data[index];
    }

    /**
     * Gets a mutable reference to a value stored in the specified index.
     * @param index the index position to search.
     * @return a constant reference to the value stored in the index.
     */
    inline T& Get(const size_t& index)
    {
        return data[index];
    }

    /**
     * @brief Returns the const raw pointer stored by the array.
     * @return the raw pointer stored by the array.
     */
    inline const T* Data() const
    {
        return data;
    }

    /**
     * @brief Returns the raw pointer stored by the array.
     * @return the raw pointer stored by the array.
     */
    inline T* Data()
    {
        return data;
    }

    /**
     * @brief Returns the size of the array in bytes.
     * @return the size of the array in bytes.
     */
    inline constexpr const size_t Size() const
    {
        return S;
    }

    /**
     * @brief Returns the size of the array in bytes.
     * @return the size of the array in bytes.
     */
    inline const size_t Count() const
    {
        return count;
    }

    // Setters

    /**
     * @brief Inserts a given element into the position specified by `index`. If the provided index
     * is larger than the array's size, an exception will be thrown.
     * @param index the position in the array we want to insert the element into.
     * @param element the element we want to store in the array.
     */
    inline void Insert(const size_t& index, const T& element)
    {
        count += !bitField.IsSet(index + 1);
        bitField.SetBit(index + 1);
        Set(index, element);
    }

    inline void Append(const T& element)
    {
        ArrayUtils::AssertIsInBounds(count, S);
        Set(count, element);
        count += !bitField.IsSet(count + 1);
        bitField.SetBit(count);
    }

    /**
     * Checks if the MSArray is empty.
     * @return true if the MSArray has had no allocations made, false if at least one element was
     * added.
     */
    inline bool Empty() const
    {
        return count == 0;
    }

    /**
     * Checks if a specified index has been assigned to. If the provided index
     * is larger than the array's size, an exception will be thrown.
     * @param index the index of the element being searched for.
     * @return true if the index has been previously assigned to.
     */
    inline bool Active(const size_t& index) const
    {
        return bitField.IsSet(index + 1);
    }

    /**
     * @brief Removes the element at the specified position from the array. If the provided index
     * is larger than the array's size, an exception will be thrown.
     * @param index the index of the element we want to remove.
     */
    inline void Remove(const size_t& index)
    {
        bitField.UnsetBit(index + 1);
        count--;
    }

    /**
     * @brief Clears the MSArray.
     */
    inline void Clear()
    {
        bitField.Clear();
        count = 0;
    }

    /**
     * Creates a base managed iterator for the array. This iterator will ignore elements which
     * have not been previously assigned. This method is slower than the FIterator but ensures
     * no garbage data is accessed
     * @return a MIter to iterate over the array
     */
    inline Utils::MIter<MSArray<T, S>, T> Iterator()
    {
        return {this};
    }

    /**
     * Creates a fast iterator type. The fast iterator iterates over every single element in an
     * array, regardless of whether they have been assigned to. This method is faster than the
     * default iterator but is less safe
     * @return an Iter instance to iterate over the array
     */
    Utils::Iter<MSArray<T, S>, T> FIterator()
    {
        return {this};
    }

private:

    /**
     * A private constructor for the MSArray. Simply initialises the count value of the array.
     * @param arrSize the amount to set the count to.
     */
    MSArray(const size_t& arrSize) : count {arrSize}
    {
        memset(data, 0, sizeof(T) * S);
    }

    /**
     * Sets the value of a given index. This function does no error checking and is susceptible to
     * indexing errors.
     * @param index the index of the value to set.
     * @param element the element that you want to set the array position to.
     */
    inline void Set(const size_t& index, const T& element)
    {
        data[index] = element;
    }

    /**
     * A function encapsulating the logic of copying two equally sized MSArrays.
     * @param other the array to copy.
     */
    inline void Copy(const MSArray& other)
    {
        ArrayUtils::CopyData(data, other.Data(), sizeof(T) * other.Size());
        bitField = other.bitField;
        count = other.count;
    }

    inline void Swap(MSArray& other)
    {
        auto tmpData = data;
        auto tmpBitField = bitField;
        auto tmpCount = count;

        memcpy(data, other.data, sizeof(T) * S);
        bitField = other.bitField;
        count = other.count;

        memcpy(other.data, tmpData, sizeof(T) * S);
        other.bitField = tmpBitField;
        other.count = tmpCount;
    }

    T data[S];
    BitUtils::SBitSet<(S / BitUtils::BYTE_SIZE_IN_BITS) + 1> bitField;
    size_t count {0};
};
} // namespace Siege::Utils

#endif // SIEGE_ENGINE_STACK_ARRAY_H
