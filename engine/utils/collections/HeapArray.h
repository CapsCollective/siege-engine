//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_HEAP_ARRAY_H
#define SIEGE_ENGINE_HEAP_ARRAY_H

#include "ArrayUtils.h"
#include "BitSet.h"
#include "Iterators.h"

namespace Siege
{
/**
 * @brief The HeapArray is a managed heap-allocated array type. The HeapArray is intended to be
 * used when a container is needed who's size is not known at compile time, but is required to be
 * constant once a size is provided
 *
 * The HeapArray tracks all data that's been inserted and removed from the array. All data inserted
 * into the array will have its indices marked as active. Accessing an index which has not been
 * activated will trigger an immediate assertion failure. As such, the HeapArray wants you to
 * only access data which has been explicitly entered into it, thus avoiding issues where 'garbage'
 * data may be accessed through
 *
 * @tparam T The data type the array will be storing
 */
template<typename T>
class MHArray
{
public:

    /**
     * @brief An empty constructor. This is invoked when no parameters are passed to the
     * constructor. Initialises the array with a size of zero. Does not allocate any space on the
     * heap. Constructing an array this way requires either an explicit resizing or a new creation
     * to add a size. Any usage of the Get() or subscript operator will cause an error
     */
    MHArray() = default;

    /**
     * @brief Initialises the HeapArray with the given size
     * @param arraySize the size of the array
     */
    explicit MHArray(size_t arraySize) :
        MHArray(arraySize, 0, BitUtils::CalculateBitSetSize(arraySize))
    {}

    /**
     * @brief Initialises the array using an initialiser list
     * @param list the initialiser list
     */
    MHArray(const std::initializer_list<T>& list)
    {
        size = list.size();
        data = ArrayUtils::Allocate<T>(sizeof(T) * size);

        size_t newByteCount = BitUtils::CalculateBitSetSize(size);

        bitField = BitUtils::BitSet(newByteCount);
        bitField.SetBitsToOne(list.size());

        ArrayUtils::CopyData(data, std::data(list), sizeof(T) * size);

        count = size;
    }

    /**
     * Creates a HeapArray from a raw array pointer
     * @param rawPtr the pointer to copy data from
     * @param ptrSize the size of the array
     */
    MHArray(const T* rawPtr, const size_t ptrSize)
    {
        size = ptrSize;
        data = ArrayUtils::Allocate<T>(sizeof(T) * size);

        size_t newByteCount = BitUtils::CalculateBitSetSize(size);

        bitField = BitUtils::BitSet(newByteCount);
        bitField.SetBitsToOne(size);

        ArrayUtils::CopyData(data, rawPtr, sizeof(T) * size);

        count = size;
    }

    /**
     * Creates an array with a pre-allocated number of indices set to active
     * @param arraySize the size of the array
     * @param allocations the number of allocations to make
     */
    MHArray(size_t arraySize, size_t allocations)
    {
        size = arraySize;
        data = ArrayUtils::Allocate<T>(sizeof(T) * size);

        size_t newByteCount = BitUtils::CalculateBitSetSize(size);

        bitField = BitUtils::BitSet(newByteCount);
        bitField.SetBitsToOne(allocations);

        memset(data, 0, sizeof(T) * allocations);

        count = allocations;
    }

    /**
     * @brief A copy constructor for the HeapArray
     * @param other the array to be copied
     */
    MHArray(const MHArray<T>& other) : size {other.size}, count {other.count}
    {
        size = other.size;

        if (other.data == nullptr && other.size == 0) return;

        AllocateMemory(size);

        ArrayUtils::CopyData(data, other.data, sizeof(T) * size);

        bitField = other.bitField;

        count = other.count;
    }

    /**
     * @brief A move constructor for the HeapArray
     * @param other the array to be moved
     */
    MHArray(MHArray<T>&& other) noexcept
    {
        Swap(other);
    }

    /**
     * @brief The destructor for the HeapArray
     */
    ~MHArray()
    {
        Destroy();
    }

    /**
     * @brief Deletes any memory that's been allocated and resets the array to default values
     */
    void Destroy()
    {
        DeallocateMemory();
        ResetValues();
    }

    /**
     * @brief An overload for the subscript operator. Gets the element at the specified index
     * @param index the index to search for
     * @return a const reference to the element at the given index
     */
    const T& operator[](size_t index) const
    {
        return Get(index);
    }

    /**
     * @brief An overload for the subscript operator. Gets the element at the specified index.
     * If the index has not previously been assigned and we use the subscript to assign a value
     * (i.e: array[0] = x), then we need to increment the count of items in the array
     * @param index the index to search for
     * @return a reference to the element at the given index
     */
    T& operator[](size_t index)
    {
        assert(data && "Trying to index into MHArray when the array has not been initialised!");
        count += !bitField.IsSet(index + 1);
        bitField.SetBit(index + 1);

        return Get(index);
    }

    /**
     * @brief Copy assignment operator. Copies the content of `other` into the current array
     * @param other the array to be copied
     * @return the array with the copied elements
     */
    MHArray<T>& operator=(const MHArray<T>& other)
    {
        // Don't copy if the objects are the same
        if (this == &other) return *this;

        Destroy();

        Copy(other);

        return *this;
    }

    /**
     * @brief Move assigment operator. Moves the contents of `other` into the current array
     * @param other the array to be moved
     * @return the new array with the moved contents
     */
    MHArray<T>& operator=(MHArray<T>&& other) noexcept
    {
        Swap(other);
        return *this;
    }

    /**
     * @brief Get a const reference to the element at the specified index
     * @param index the index of the element we want to access
     * @return the element (if found)
     */
    const T& Get(size_t index) const
    {
        BitUtils::AssertIsSet(bitField.BitField(), index);

        return data[index];
    }

    /**
     * @brief Get a reference to the element at the specified index
     * @param index the index of the element we want to access
     * @return the element (if found)
     */
    T& Get(size_t index)
    {
        return data[index];
    }

    /**
     * @brief Inserts a given element into the position specified by `index`.
     * @param index the position in the array we want to insert the element into.
     * @param element the element we want to store in the array.
     */
    void Insert(size_t index, const T& element)
    {
        SetActive(index);

        Set(index, element);
    }

    /**
     * Appends a value to the end of the collection
     * @param element the value to add
     */
    inline void Append(const T& element)
    {
        ArrayUtils::AssertIsInBounds(count, size);
        Set(count, element);
        count += !bitField.IsSet(count + 1);
        bitField.SetBit(count);
    }

    /**
     * @brief Removes the element at the specified position from the array
     * is larger than the array's size, an exception will be thrown
     * @param index the index of the element we want to remove
     */
    void Remove(size_t index)
    {
        bitField.UnsetBit(index + 1);
        count--;
    }

    /**
     * @brief Resizes the array. Re-allocates the memory to a new size and adjusts the stateMask
     * accordingly
     * @param newSize the new size of the array
     */
    void Resize(size_t newSize)
    {
        // Setting the array to 0 is the equivalent of destroying the array.
        if (newSize == 0)
        {
            Destroy();
            return;
        }

        // Every 8 positions are represented with one 8-bit unsigned integer. As such, every 8
        // elements in the array represents one to the size of our collection of byte masks.
        size_t newByteCount = ((newSize / BitUtils::BYTE_SIZE_IN_BITS) + 1);

        // Reallocate the two pointers
        ReallocateMemory(newSize);

        // Resize our state mask. Reallocating the states simply changes the number of bytes.
        // We need to change the last byte available to us to represent the new number of possible
        // states.
        bitField.Resize(newByteCount);
        bitField.UnsetPostBits(newSize);

        size = newSize;

        // Adjust the count to be within the range of our new size.
        count = ArrayUtils::SetCount(count, 0, newSize);
    }

    /**
     * @brief Checks if an array element has been filled
     * @param index the index of the element we want to check
     * @return `true` if this element has been set, `false` if it has not
     */
    bool IsActive(size_t index) const
    {
        return bitField.IsSet(index + 1);
    }

    /**
     * @brief Clears the HeapArray
     */
    void Clear()
    {
        count = 0;
        bitField.Clear();
    }

    /**
     * @brief Returns the count of elements in the array
     * @return the number of filled elements in the array
     */
    const size_t& Count() const
    {
        return count;
    }

    /**
     * @brief Returns the size of the array in bytes
     * @return the size of the array in bytes
     */
    const size_t& Size() const
    {
        return size;
    }

    /**
     * @brief Returns the const raw pointer stored by the array
     * @return the raw pointer stored by the array
     */
    const T* Data() const
    {
        return data;
    }

    /**
     * @brief Returns the raw pointer stored by the array
     * @return the raw pointer stored by the array
     */
    T* Data()
    {
        return data;
    }

    /**
     * Creates a base managed iterator for the array. This iterator will ignore elements which
     * have not been previously assigned. This method is slower than the CreateFIterator but ensures
     * no garbage data is accessed
     * @return a MIter to iterate over the array
     */
    inline MIter<MHArray<T>, T> CreateIterator()
    {
        return {this};
    }

    /**
     * Creates a base managed iterator for the array. This iterator will ignore elements which
     * have not been previously assigned. This method is slower than the CreateFIterator but ensures
     * no garbage data is accessed
     * @return a MIter to iterate over the array
     */
    inline CMIter<MHArray<T>, T> CreateIterator() const
    {
        return {this};
    }

    /**
     * Creates a fast iterator type. The fast iterator iterates over every single element in an
     * array, regardless of whether they have been assigned to. This method is faster than the
     * default iterator but is less safe
     * @return an Iter instance to iterate over the array
     */
    inline ConstIter<MHArray<T>, T> CreateFIterator() const
    {
        return {this};
    }

    /**
     * Creates a fast iterator type. The fast iterator iterates over every single element in an
     * array, regardless of whether they have been assigned to. This method is faster than the
     * default iterator but is less safe
     * @return an Iter instance to iterate over the array
     */
    inline Iter<MHArray<T>, T> CreateFIterator()
    {
        return {this};
    }

    /**
     * Returns the element in the MSArray
     * @return the last element in the array
     */
    inline const T& Back() const
    {
        return data[bitField.LeftMostBit() - 1];
    }

    /**
     * Returns the element in the MSArray
     * @return the last element in the array
     */
    inline T& Back()
    {
        return data[bitField.LeftMostBit() - 1];
    }

private:

    // Private Constructors

    /**
     * @brief The primary constructor for the HeapArray.
     * @param newSize the size we want to allocate for the array.
     * @param newCount the number of objects in the array.
     * @param newBytes the number of bytes to allocate for tracking state.
     * @param masks a raw pointer to a set of 8-bit unsigned integers.
     * @param data a raw pointer to the data stored in the array.
     */
    MHArray(size_t newSize, size_t newCount, size_t newBytes, T* data) :
        size {newSize},
        count {newCount},
        bitField {BitUtils::BitSet(newBytes)},
        data {data}
    {
        memset(data, 0, sizeof(T) * size);
    }

    /**
     * @brief A constructor for creating a HeapArray with a number of sizes.
     * @param newSize the size of the array.
     * @param newCount the number of elements in the array.
     * @param masksSize the number of bytes to allocate for storing our states.
     */
    MHArray(size_t newSize, size_t newCount, size_t masksSize) :
        MHArray(newSize, newCount, masksSize, ArrayUtils::Allocate<T>(sizeof(T) * newSize))
    {}

    // Functions

    /**
     * @brief Sets an element in the array to a corresponding value
     * @param index the index to insert the element into
     * @param value the element to insert
     */
    void Set(size_t index, const T& value)
    {
        data[index] = value;
    }

    void SetActive(uint32_t index)
    {
        count += !bitField.IsSet(index + 1);
        bitField.SetBit(index + 1);
    }

    /**
     * @brief Allocates our raw pointers to the specified values.
     * @param arraySize the size to allocate `data` to.
     * @param bytesCount the number of bytes to assign for states.
     */
    void AllocateMemory(size_t arraySize)
    {
        data = ArrayUtils::Allocate<T>(sizeof(T) * arraySize);
    }

    /**
     * Reallocates the memory for both the bitmasks and the arrays.
     * @param arraySize the number of elements in the array.
     * @param bytesCount the number of bytes to allocate.
     */
    void ReallocateMemory(size_t arraySize)
    {
        data = ArrayUtils::Reallocate<T>(data, sizeof(T) * arraySize);
    }

    /**
     * @brief Swaps the data between two HeapArrays.
     * @param other the HeapArray to swap with.
     */
    void Swap(MHArray<T>& other)
    {
        auto tmpSize = size;
        auto tmpCount = count;
        auto tmpData = data;
        auto tmpMask = bitField;

        size = other.size;
        count = other.count;
        data = other.data;
        bitField = other.bitField;

        other.data = tmpData;
        other.bitField = tmpMask;
        other.size = tmpSize;
        other.count = tmpCount;
    }

    /**
     * @brief Frees the memory allocated by the array
     */
    void DeallocateMemory()
    {
        free(data);
        bitField.Clear();
    }

    /**
     * @brief Resets all counters to their default values
     */
    void ResetValues()
    {
        data = nullptr;
        bitField.Clear();
        size = count = 0;
    }

    /**
     * @brief Copies the elements from one HeapArray to another
     * @param other the other array to copy elements from
     */
    void Copy(const MHArray<T>& other)
    {
        if (other.size == 0 && other.data == nullptr) return;

        count = other.count;
        size = other.size;

        ReallocateMemory(size);

        ArrayUtils::CopyData(data, other.data, sizeof(T) * size);
        bitField = other.bitField;
    }

    // Tracker Variables
    size_t size {0};
    size_t count {0};

    // Pointers
    BitUtils::BitSet bitField;
    T* data {nullptr};
};

} // namespace Siege

#endif // SIEGE_ENGINE_HEAP_ARRAY_H
