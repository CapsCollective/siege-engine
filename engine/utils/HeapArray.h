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

namespace Siege::Utils
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
class HeapArray
{
public:

    /**
     * @brief A base iterator for the HeapArray class. This iterator moves through all active
     * elements in the array and returns them. As such, all data points not explicitly inserted
     * into the array will be ignored
     */
    class Iterator
    {
    public:

        typedef void (Iterator::*BoolType)() const;

        // TODO(Aryeh): Add more operators as needed (--, ->, etc).
        /**
         * @brief Iterator constructor
         * @param arrPtr the pointer to the HeapArray
         */
        inline Iterator(HeapArray<T>* arrPtr) : ptr {arrPtr}
        {
            ptr = arrPtr->Data() ? arrPtr : nullptr;
        }

        /**
         * @brief The iterator prefix increment operator. This operator will increment the pointer
         * as many times as needed to find an active element. As such, it will not return inactive
         * array elements
         * @return the iterator with the pointer and index incremented
         */
        inline Iterator& operator++()
        {
            // If the next element in the array is invalid, keep incrementing until we find one that
            // is
            while (ptr->Data() && !ptr->Active(index + 1)) index++;

            // TODO: Profile if using a reinterpret cast would be faster here
            ptr = index < ptr->Size() ? ptr : 0;

            index++;

            return *this;
        }

        /**
         * @brief The dereference operator
         * @return the de-referenced pointer value
         */
        inline T& operator*()
        {
            return *(ptr->Data() + index);
        }

        /**
         * Checks if the Iterator if valid or not
         * @return true of the Iterator is valid, false otherwise
         */
        inline operator BoolType() const
        {
            return ptr ? &Iterator::DoNothing : 0;
        }

        /**
         * @brief An equal operator, checks with equality
         * @param other the other iterator to compare to
         * @return `true` if the pointers stored by both iterators are the same
         */
        inline bool operator==(const Iterator& other) const
        {
            return ptr->Data() == other.ptr->Data() && index == other.index;
        }

        /**
         * @brief A non equality operator. Checks that two iterators are not equal
         * @param other the iterator to compare to
         * @return `true` if the pointer addresses stored by both iterators are not the same
         */
        inline bool operator!=(const Iterator& other) const
        {
            return ptr->Data() != other.ptr->Data() && index != other.index;
        }

    private:

        /**
         * A private function which does nothing (used for implementing the safe bool idiom)
         */
        inline void DoNothing() const {};

        size_t index {0};
        HeapArray<T>* ptr {nullptr};
    };

    /**
     * @brief An empty constructor. This is invoked when no parameters are passed to the
     * constructor. Initialises the array with a size of zero. Does not allocate any space on the
     * heap. Constructing an array this way requires either an explicit resizing or a new creation
     * to add a size. Any usage of the Get() or subscript operator will cause an error
     */
    HeapArray() = default;

    /**
     * @brief Initialises the HeapArray with the given size
     * @param arraySize the size of the array
     */
    explicit HeapArray(const size_t& arraySize) :
        HeapArray(arraySize, 0, BitUtils::CalculateBitSetSize(arraySize))
    {}

    /**
     * @brief Initialises the array using an initialiser list
     * @param list the initialiser list
     */
    HeapArray(const std::initializer_list<T>& list)
    {
        size = list.size();
        data = ArrayUtils::Allocate<T>(sizeof(T) * size);

        size_t newByteCount = BitUtils::CalculateBitSetSize(size);

        bitField = BitUtils::BitSet(newByteCount);
        bitField.SetBitsToOne(list.size());

        ArrayUtils::CopyData(data, std::data(list), sizeof(T) * size);

        count = size;
    }

    HeapArray(const T* rawPtr, const size_t ptrSize)
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
     * @brief A copy constructor for the HeapArray
     * @param other the array to be copied
     */
    HeapArray(const HeapArray<T>& other) : size {other.size}, count {other.count}
    {
        size = other.size;

        if (other.data == nullptr && other.size == 0) return;

        auto stateMaskCount = BitUtils::CalculateBitSetSize(other.size);

        AllocateMemory(size, stateMaskCount);

        ArrayUtils::CopyData(data, other.data, sizeof(T) * size);

        bitField = other.bitField;

        count = other.count;
    }

    /**
     * @brief A move constructor for the HeapArray
     * @param other the array to be moved
     */
    HeapArray(HeapArray<T>&& other) noexcept
    {
        Swap(std::move(other));
    }

    /**
     * @brief The destructor for the HeapArray
     */
    ~HeapArray()
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
    const T& operator[](const size_t& index) const
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
    T& operator[](const size_t& index)
    {
        count += !bitField.IsSet(index + 1);
        bitField.SetBit(index + 1);

        return Get(index);
    }

    /**
     * @brief Copy assignment operator. Copies the content of `other` into the current array
     * @param other the array to be copied
     * @return the array with the copied elements
     */
    HeapArray<T>& operator=(const HeapArray<T>& other)
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
    HeapArray<T>& operator=(HeapArray<T>&& other) noexcept
    {
        if (this == &other) return *this;

        // Deallocate all existing memory
        Destroy();

        Swap(std::move(other));

        return *this;
    }

    /**
     * @brief Get a const reference to the element at the specified index
     * @param index the index of the element we want to access
     * @return the element (if found)
     */
    const T& Get(const size_t& index) const
    {
        BitUtils::AssertIsSet(bitField.BitField(), size);

        return data[index];
    }

    /**
     * @brief Get a reference to the element at the specified index
     * @param index the index of the element we want to access
     * @return the element (if found)
     */
    T& Get(const size_t& index)
    {
        return data[index];
    }

    /**
     * @brief Inserts a given element into the position specified by `index`.
     * @param index the position in the array we want to insert the element into.
     * @param element the element we want to store in the array.
     */
    void Insert(const size_t index, const T& element)
    {
        count += !bitField.IsSet(index + 1);
        bitField.SetBit(index + 1);

        Set(index, element);
    }

    /**
     * @brief Removes the element at the specified position from the array
     * is larger than the array's size, an exception will be thrown
     * @param index the index of the element we want to remove
     */
    void Remove(const size_t& index)
    {
        bitField.UnsetBit(index + 1);
        count--;
    }

    /**
     * @brief Resizes the array. Re-allocates the memory to a new size and adjusts the stateMask
     * accordingly
     * @param newSize the new size of the array
     */
    void Resize(const size_t& newSize)
    {
        // Setting the array to 0 is the equivalent of destroying the array.
        if (newSize == 0)
        {
            Destroy();
            return;
        }

        // Every 8 positions are represented with one 8 bit unsigned integer. As such, every 8
        // elements in the array represents one to the size of our collection of byte masks.
        size_t newByteCount = ((newSize / BitUtils::BYTE_SIZE_IN_BITS) + 1);

        // Reallocate the two pointers
        ReallocateMemory(newSize, newByteCount);

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
    bool Active(const size_t& index)
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

    Iterator CreateIterator()
    {
        return {this};
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
    HeapArray(const size_t& newSize, const size_t& newCount, const size_t& newBytes, T* data) :
        size {newSize},
        count {newCount},
        bitField {BitUtils::BitSet(newBytes)},
        data {data}
    {}

    /**
     * @brief A constructor for creating a HeapArray with a number of sizes.
     * @param newSize the size of the array.
     * @param newCount the number of elements in the array.
     * @param masksSize the number of bytes to allocate for storing our states.
     */
    HeapArray(const size_t& newSize, const size_t& newCount, const size_t& masksSize) :
        HeapArray(newSize, newCount, masksSize, ArrayUtils::Allocate<T>(sizeof(T) * newSize))
    {}

    // Functions

    /**
     * @brief Sets an element in the array to a corresponding value
     * @param index the index to insert the element into
     * @param value the element to insert
     */
    void Set(const size_t& index, const T& value)
    {
        data[index] = value;
    }

    /**
     * @brief Allocates our raw pointers to the specified values.
     * @param arraySize the size to allocate `data` to.
     * @param bytesCount the number of bytes to assign for states.
     */
    void AllocateMemory(const size_t& arraySize, const size_t& bytesCount)
    {
        data = ArrayUtils::Allocate<T>(sizeof(T) * arraySize);
    }

    /**
     * Reallocates the memory for both the bitmasks and the arrays.
     * @param arraySize the number of elements in the array.
     * @param bytesCount the number of bytes to allocate.
     */
    void ReallocateMemory(const size_t& arraySize, const size_t& bytesCount)
    {
        data = ArrayUtils::Reallocate<T>(data, sizeof(T) * arraySize);
    }

    /**
     * @brief Swaps the data between two HeapArrays.
     * @param other the HeapArray to swap with.
     */
    void Swap(HeapArray&& other)
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
    void Copy(const HeapArray<T>& other)
    {
        if (other.size == 0 && other.data == nullptr) return;

        count = other.count;
        size = other.size;

        auto byteCount = BitUtils::CalculateBitSetSize(other.size);

        ReallocateMemory(size, byteCount);

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

} // namespace Siege::Utils

#endif // SIEGE_ENGINE_HEAP_ARRAY_H
