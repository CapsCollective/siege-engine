//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_HEAP_ARRAY_H
#define SIEGE_ENGINE_HEAP_ARRAY_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <utility>

namespace Siege::Utils
{

// Static methods

/**
 * @brief Allocates a chunk of memory and returns it as a pointer to a type.
 * @tparam T the type of the pointer to allocate.
 * @param dataSize the size of the memory we want to allocate.
 * @return the pointer we allocated.
 */
template<typename T>
static inline T* Allocate(const size_t& dataSize)
{
    return static_cast<T*>(malloc(dataSize));
}

/**
 * @brief Reallocate a chunk of memory to a new size.
 * @tparam T the type of the pointer.
 * @param ptr the pointer to be reallocated.
 * @param dataSize the size of the new memory chunk.
 * @return the new reallocated pointer.
 */
template<typename T>
static inline T* Reallocate(T* ptr, const size_t& dataSize)
{
    return static_cast<T*>(realloc(ptr, dataSize));
}

// HeapArray

/**
 * @brief The HeapArray is a managed heap-allocated array type. The HeapArray is intended to be
 * used when a container is needed who's size is not known at compile time, but is required to be
 * constant once a size is provided.
 *
 * The HeapArray tracks all data that's been inserted and removed from the array. All data inserted
 * into the array will have its indexed marked as active. Accessing an index which has not been
 * activated will trigger an immediate assertion failure. As such, the HeapArray wants you to
 * only access data which has been explicitly entered into it, thus avoiding issues where 'garbage'
 * data may be accessed through.
 *
 * @tparam T The data type the array will be storing.
 */
template<typename T>
class HeapArray
{
public:

    /**
     * @brief A base iterator for the HeapArray class. This iterator moves through all active
     * elements in the array and returns them. As such, all data points not explicitly inserted
     * into the array will be ignored.
     */
    class Iterator
    {
    public:

        // TODO(Aryeh): Add more operators as needed (--, ->, etc).
        /**
         * @brief Iterator constructor.
         * @param valuePointer the raw array pointer.
         * @param newMaskPointer the pointer to our state array.
         * @param newIndex the starting index.
         * @param newSize the array size.
         */
        explicit Iterator(T* valuePointer,
                          uint8_t* newMaskPointer,
                          const size_t& newIndex,
                          const size_t& newSize) :
            pointer {valuePointer},
            maskPointer {newMaskPointer},
            index {newIndex},
            size {newSize}
        {}

        /**
         * @brief The iterator prefix increment operator. This operator will increment the pointer
         * as many times as needed to find an active element. As such, it will not return unactive
         * array elements.
         * @return the iterator with the pointer and index incremented.
         */
        Iterator& operator++()
        {
            // If the next element in the array is invalid, keep incrementing until we find one that
            // is.

            while (IsNextIndexInvalid()) Increment();

            Increment();

            return *this;
        }

        /**
         * @brief The postfix increment operator. Does the same thing as the postfix operator, but
         * applies it after returning the object.
         * @return the iterator before it was incremented.
         */
        const Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++(*this);
            return iterator;
        }

        /**
         * @brief The dereference operator.
         * @return the de-referenced pointer value.
         */
        T& operator*()
        {
            return *pointer;
        }

        /**
         * @brief Increments the pointer and index
         */
        void Increment()
        {
            pointer++;
            index++;
        }

        /**
         * @brief Checks the next index to see if it's active.
         * @return `true` if the next index is inactive, `false` if it is.
         */
        bool IsNextIndexInvalid()
        {
            // Get the chunk of bytes we want to use to search for states.
            uint8_t byteMask = maskPointer[(index + 1) / BYTE_SIZE_IN_BITS];

            /**
             * Get a bit representation of the position we want to search for.
             * For example, given a bit position in index 1, we create a bit mask with a value of
             * 1 left shifted by 1 position (0000 0010). Finally, since our bytes contain 8 bits,
             * we want to make sure that we just check the index we get is relative to our byte.
             * i.e: 2 % 8 = 2 or 9 % 8 = 1. No matter how large the index, we'll always get its
             * position in 8s.
             */
            auto bitPosition = (1 << ((index + 1) % BYTE_SIZE_IN_BITS));

            /**
             * Run a bitwise AND operation to check if the corresponding byte exists in the original
             * mask. i.e: 1010 0010 & 0000 0010 will return 1 (since 1 exists in the second bit).
             *
             * Finally, we also want to make sure the next index is in bounds.
             */
            return (byteMask & bitPosition) == 0 && (index + 1) < size;
        }

        /**
         * @brief An equal operator, checks with equality.
         * @param other the other iterator to compare to.
         * @return `true` if the pointers stored by both iterators are the same.
         */
        bool operator==(const Iterator& other) const
        {
            return pointer == other.pointer;
        }

        /**
         * @brief A non equality operator. Checks that twio iterators are not equal.
         * @param other the iterator to compare to.
         * @return `true` if the pointer addresses stored by both iterators are not the same.
         */
        bool operator!=(const Iterator& other) const
        {
            return pointer != other.pointer;
        }

    private:

        T* pointer;
        uint8_t* maskPointer;
        size_t index;
        size_t size;
    };

    /**
     * @brief An empty constructor. This is invoked when no parameters are passed to the
     * constructor. Initialises the array with a size of zero. Does not allocate any space on the
     * heap. Constructing an array this way requires either an explicit resizing or a new creation
     * to add a size. Any usage of the Get() or subscript operator will cause an error.
     */
    HeapArray() : HeapArray(0, 0, 0, nullptr, nullptr) {}

    /**
     * @brief Initialises the HeapArray with the given size.
     * @param arraySize the size of the array.
     */
    explicit HeapArray(const size_t& arraySize) :
        HeapArray(arraySize, 0, CalculateByteMaskCount(arraySize))
    {}

    /**
     * @brief Initialises the array using an initialiser list.
     * @param list the initialiser list.
     */
    HeapArray(const std::initializer_list<T>& list)
    {
        size = list.size();

        data = Allocate<T>(sizeof(T) * size);

        size_t newByteCount = CalculateByteMaskCount(size);

        stateMaskBitfield = Allocate<uint8_t>(BYTE_MASK_SIZE * newByteCount);

        memset(stateMaskBitfield, 0, (BYTE_MASK_SIZE * newByteCount));

        CopyData(std::data(list), sizeof(T) * size);

        uint8_t bitsToProcess = size;

        for (size_t i = 0; i < newByteCount; i++)
        {
            bool isFullByte = (bitsToProcess - BYTE_SIZE_IN_BITS) >= 0;

            uint8_t maskBits = (bitsToProcess * !isFullByte) + (BYTE_SIZE_IN_BITS * isFullByte);

            stateMaskBitfield[i] =
                MAX_BIT_VALUES[std::clamp<size_t>(maskBits - 1, 0, BYTE_SIZE_IN_BITS)];

            bitsToProcess -= BYTE_SIZE_IN_BITS;
        }

        count = size;
    }

    /**
     * @brief A copy constructor for the HeapArray.
     * @param other the array to be copied.
     */
    HeapArray(const HeapArray<T>& other) : size {other.size}, count {other.count}
    {
        if (other.data == nullptr && other.size == 0) return;

        auto stateMaskCount = CalculateByteMaskCount(other.size);

        AllocateMemory(size, stateMaskCount);

        CopyData(other.data, sizeof(T) * size);
        CopyMasks(other.stateMaskBitfield, BYTE_MASK_SIZE * stateMaskCount);

        count = other.Count();
    }

    /**
     * @brief A move constructor for the HeapArray.
     * @param other the array to be moved.
     */
    HeapArray(HeapArray<T>&& other) noexcept :
        size {std::move(other.size)},
        count {std::move(other.count)},
        stateMaskBitfield {std::move(other.stateMaskBitfield)},
        data {std::move(other.data)}
    {
        other.ResetValues();
    }

    /**
     * @brief The destructor for the HeapArray.
     */
    ~HeapArray()
    {
        Destroy();
    }

    /**
     * @brief Deletes any memory that's been allocated and resets the array to default values.
     */
    void Destroy()
    {
        DeallocateMemory();
        ResetValues();
    }

    /**
     * @brief An overload for the subscript operator. Gets the element at the specified index.
     * @param index the index to search for.
     * @return a const reference to the element at the given index.
     */
    const T& operator[](const size_t& index) const
    {
        return Get(index);
    }

    /**
     * @brief An overload for the subscript operator. Gets the element at the specified index.
     * If the index has not previously been assigned and we use the subscript to assign a value
     * (i.e: array[0] = x), then we need to increment the count of items in the array.
     * @param index the index to search for.
     * @return a reference to the element at the given index.
     */
    T& operator[](const size_t& index)
    {
        VerifyIndex(index);
        return Get(index);
    }

    /**
     * @brief Copy assignment operator. Copies the content of `other` into the current array.
     * @param other the array to be copied.
     * @return the array with the copied elements.
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
     * @brief Move assigment operator. Moves the contents of `other` into the current array.
     * @param other the array to be moved.
     * @return the new array with the moved contents.
     */
    HeapArray<T>& operator=(HeapArray<T>&& other) noexcept
    {
        if (this == &other) return *this;

        // Deallocate all existing memory
        Destroy();

        // Move everything
        data = std::move(other.data);
        stateMaskBitfield = std::move(other.stateMaskBitfield);
        size = std::move(other.size);
        count = std::move(other.count);

        // Set other back to default state (we don't want it to deallocate the pointers we just
        // moved on destruction)
        other.ResetValues();

        return *this;
    }

    /**
     * @brief Get a const reference to the element at the specified index.
     * @param index the index of the element we want to access.
     * @return the element (if found).
     */
    const T& Get(const size_t& index) const
    {
        CheckIfActive(index);

        return data[index];
    }

    /**
     * @brief Get a reference to the element at the specified index.
     * @param index the index of the element we want to access.
     * @return the element (if found).
     */
    T& Get(const size_t& index)
    {
        CheckIfActive(index);
        return data[index];
    }

    /**
     * @brief Inserts a given element into the position specified by `index`. If this element
     * @param index the position in the array we want to insert the element into.
     * @param element the element we want to store in the array.
     */
    void Insert(const size_t index, const T& element)
    {
        VerifyIndex(index);
        Set(index, element);
    }

    /**
     * @brief Removes the element at the specified position from the array. If the provided index
     * is larger than the array's size, an exception will be thrown.
     * @param index the index of the element we want to remove.
     */
    void Remove(const size_t& index)
    {
        CheckIfIndexInBounds(index);
        stateMaskBitfield[index / BYTE_SIZE_IN_BITS] &= ~(1 << (index % BYTE_SIZE_IN_BITS));
        count--;
    }

    /**
     * @brief Checks if the index is in bounds of the array size.
     * @param index the index we want to check.
     * @return a boolean specifying if the index is in bounds.
     */
    bool IsInBounds(const size_t& index)
    {
        return index < size;
    }

    /**
     * @brief Resizes the array. Re-allocates the memory to a new size and adjusts the stateMask
     * accordingly.
     * @param newSize the new size of the array.
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
        size_t newByteCount = ((newSize / BYTE_SIZE_IN_BITS) + 1);

        // Reallocate the two pointers
        ReallocateMemory(newSize, newByteCount);

        // Resize our state mask. Reallocating the states simply changes the number of bytes.
        // We need to change the last byte available to us to represent the new number of possible
        // states.
        ResizeStateMask(newSize, newByteCount);

        size = newSize;

        // Adjust the count to be within the range of our new size.
        count = std::clamp<size_t>(count, 0, newSize);
    }

    /**
     * @brief Checks if an array element has been filled.
     * @param index the index of the element we want to check.
     * @return `true` if this element has been set, `false` if it has not.
     */
    bool Active(const size_t& index)
    {
        // Get the byte chunk that our index falls into
        auto byteMask = stateMaskBitfield[index / BYTE_SIZE_IN_BITS];

        /**
         * Get a bit representation of the position we want to search for.
         * For example, given a bit position in index 1, we create a bit mask with a value of
         * 1 left shifted by 1 position (0000 0010). Finally, since our bytes contain 8 bits,
         * we want to make sure that we just check the index we get is relative to our byte.
         * i.e: 2 % 8 = 2 or 9 % 8 = 1. No matter how large the index, we'll always get its
         * position in 8s.
         */
        return (byteMask & (1 << (index % BYTE_SIZE_IN_BITS))) != 0;
    }

    /**
     * @brief Clears the HeapArray.
     */
    void Clear()
    {
        count = 0;
        memset(stateMaskBitfield, 0, (BYTE_MASK_SIZE * CalculateByteMaskCount(size)));
    }

    /**
     * @brief Returns the count of elements in the array.
     * @return the number of filled elements in the array.
     */
    const size_t& Count() const
    {
        return count;
    }

    /**
     * @brief Returns the size of the array in bytes.
     * @return the size of the array in bytes.
     */
    const size_t& Size() const
    {
        return size;
    }

    /**
     * @brief Returns the const raw pointer stored by the array.
     * @return the raw pointer stored by the array.
     */
    const T* Data() const
    {
        return data;
    }

    /**
     * @brief Returns the raw pointer stored by the array.
     * @return the raw pointer stored by the array.
     */
    T* Data()
    {
        return data;
    }

    /**
     * @brief Creates an iterator pointing to the start of the HeapArray.
     * @return an Iterator set to the start of the array.
     */
    Iterator begin()
    {
        return Iterator(data, stateMaskBitfield, 0, size);
    }

    /**
     * @brief Creates an iterator pointing to the end of the HeapArray.
     * @return an Iterator set to the end of the array.
     */
    Iterator end()
    {
        return Iterator(data + size, stateMaskBitfield, size, size);
    }

    /**
     * @brief Creates a const iterator pointing to the start of the HeapArray.
     * @return an Iterator set to the start of the array.
     */
    const Iterator begin() const
    {
        return Iterator(data, stateMaskBitfield, 0, size);
    }

    /**
     * @brief Creates a const iterator pointing to the end of the HeapArray.
     * @return an Iterator set to the start of the array.
     */
    const Iterator end() const
    {
        return Iterator(data + size, stateMaskBitfield, size, size);
    }

private:

    // Class constants

    // The number of states we can fit per segment (1 byte = 8 states)
    static constexpr uint8_t BYTE_SIZE_IN_BITS = 8;
    // The default object size for each segment (1 byte)
    static constexpr uint8_t BYTE_MASK_SIZE = sizeof(uint8_t);
    // Each position represents a byte where all bits before the index are 1.
    // i.e: index 2 = 0000 00111
    static constexpr uint8_t MAX_BIT_VALUES[BYTE_SIZE_IN_BITS] = {1, 3, 7, 15, 31, 63, 127, 255};

    // Private Constructors

    /**
     * @brief The primary constructor for the HeapArray.
     * @param newSize the size we want to allocate for the array.
     * @param newCount the number of objects in the array.
     * @param newBytes the number of bytes to allocate for tracking state.
     * @param masks a raw pointer to a set of 8-bit unsigned integers.
     * @param data a raw pointer to the data stored in the array.
     */
    HeapArray(const size_t& newSize,
              const size_t& newCount,
              const size_t& newBytes,
              uint8_t* masks,
              T* data) :
        size {newSize},
        count {newCount},
        stateMaskBitfield {masks},
        data {data}
    {}

    /**
     * @brief A constructor for creating a HeapArray with a number of sizes.
     * @param newSize the size of the array.
     * @param newCount the number of elements in the array.
     * @param masksSize the number of bytes to allocate for storing our states.
     */
    HeapArray(const size_t& newSize, const size_t& newCount, const size_t& masksSize) :
        HeapArray(newSize,
                  newCount,
                  masksSize,
                  Allocate<uint8_t>(BYTE_MASK_SIZE * masksSize),
                  Allocate<T>(sizeof(T) * newSize))
    {}

    // Functions

    /**
     * @brief Sets an element in the array to a corresponding value.
     * @param index the index to insert the element into.
     * @param value the element to insert.
     */
    void Set(const size_t& index, const T& value)
    {
        data[index] = value;
    }

    /**
     * @brief Verifies the index is active. If it is not active, increment our count and set it to
     * active.
     * @param index the index to search for.
     */
    void VerifyIndex(const size_t& index)
    {
        CheckIfIndexInBounds(index);
        count += !Active(index);
        stateMaskBitfield[index / BYTE_SIZE_IN_BITS] |= (1 << (index % BYTE_SIZE_IN_BITS));
    }

    /**
     * @brief Checks if the index is within bounds of the array. If it is not, an assertion failure
     * will be triggered.
     * @param index the index to evaluate.
     */
    void CheckIfIndexInBounds(const size_t& index)
    {
        assert(IsInBounds(index) && "The index provided is larger than the size of the array!");
    }

    /**
     * @brief Checks if an array element is active. If it is not active, an assertion failure will
     * be triggered.
     * @param index the index to evaluate.
     */
    void CheckIfActive(const size_t& index)
    {
        assert(Active(index) && "Element does not exist in the array!");
    }

    size_t CalculateByteMaskCount(const size_t& arraySize)
    {
        return ((arraySize / BYTE_SIZE_IN_BITS) + 1);
    }

    /**
     * @brief Allocates our raw pointers to the specified values.
     * @param arraySize the size to allocate `data` to.
     * @param bytesCount the number of bytes to assign for states.
     */
    void AllocateMemory(const size_t& arraySize, const size_t& bytesCount)
    {
        data = Allocate<T>(sizeof(T) * arraySize);
        stateMaskBitfield = Allocate<uint8_t>(BYTE_MASK_SIZE * bytesCount);
    }

    /**
     * Reallocates the memory for both the bitmasks and the arrays.
     * @param arraySize the number of elements in the array.
     * @param bytesCount the number of bytes to allocate.
     */
    void ReallocateMemory(const size_t& arraySize, const size_t& bytesCount)
    {
        data = Reallocate<T>(data, sizeof(T) * arraySize);
        stateMaskBitfield = Reallocate<uint8_t>(stateMaskBitfield, BYTE_MASK_SIZE * bytesCount);
    }

    /**
     * @brief Copies data into the `data` pointer.
     * @param source the pointer to copy data from.
     * @param dataSize the amount of data to be copied.
     */
    void CopyData(const T* source, const size_t& dataSize)
    {
        memcpy(data, source, dataSize);
    }

    /**
     * @brief Copies the byte masks from one pointer to another.
     * @param source the pointer to copy data from.
     * @param size the size of the memory to be copied.
     */
    void CopyMasks(const uint8_t* source, const size_t& byteSize)
    {
        memcpy(stateMaskBitfield, source, byteSize);
    }

    /**
     * @brief Resizes the state mask in accordance to the new array size.
     * @param newSize the new array size.
     * @param newByteCount the new number of byte masks.
     */
    void ResizeStateMask(const size_t& newSize, const size_t& newByteCount)
    {
        // Get the last byte mask since it is the only one needed to change.
        size_t bitsToProcess = newSize - ((newByteCount - 1) * 8);

        // Get the number of bits that need to be checked for equality.
        // i.e: if we have a mask: 0001 0101 in an array of size 5, and we resize it to only have 4
        // elements, we want to make sure that the fifth bit in the mask is reset. As such, we want
        // to make sure that we reset all bits EXCEPT for the first four bits.
        size_t bitIndex = std::clamp<size_t>(bitsToProcess - 1, 0, bitsToProcess);

        // Use an AND operation to reset the byte mask to the position that we want.
        stateMaskBitfield[CalculateByteMaskCount(newSize) - 1] &= MAX_BIT_VALUES[bitIndex];
    }

    /**
     * @brief Frees the memory allocated by the array.
     */
    void DeallocateMemory()
    {
        free(data);
        free(stateMaskBitfield);
    }

    /**
     * @brief Resets all counters to their default values.
     */
    void ResetValues()
    {
        data = nullptr;

        stateMaskBitfield = nullptr;

        size = count = 0;
    }

    /**
     * @brief Copies the elements from one HeapArray to another.
     * @param other the other array to copy elements from.
     */
    void Copy(const HeapArray<T>& other)
    {
        if (other.size == 0 && other.data == nullptr) return;

        count = other.count;
        size = other.size;

        auto byteCount = CalculateByteMaskCount(other.size);

        ReallocateMemory(size, byteCount);

        CopyData(other.data, sizeof(T) * size);
        CopyMasks(other.stateMaskBitfield, BYTE_MASK_SIZE * byteCount);
    }

    // Tracker Variables
    size_t size {0};
    size_t count {0};

    // Pointers
    uint8_t* stateMaskBitfield {nullptr};
    T* data {nullptr};
};

} // namespace Siege::Utils

#endif // SIEGE_ENGINE_HEAP_ARRAY_H
