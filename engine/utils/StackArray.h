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
        inline T& operator*() { return *pointer; }
        /**
         * @brief An equal operator, checks with equality.
         * @param other the other iterator to compare to.
         * @return `true` if the pointers stored by both iterators are the same.
         */
        inline bool operator==(const Iterator& other) const { return pointer == other.pointer; }

        /**
         * @brief A non equality operator. Checks that twio iterators are not equal.
         * @param other the iterator to compare to.
         * @return `true` if the pointer addresses stored by both iterators are not the same.
         */
        inline bool operator!=(const Iterator& other) const { return pointer != other.pointer; }

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
     * @brief A base iterator for the Managed Stack Array (MSArray) class. This iterator moves through all active
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
                          const size_t& newIndex)
            : pointer {valuePointer}, maskPointer {newMaskPointer}, index {newIndex}
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
            return (byteMask & bitPosition) == 0 && (index + 1) < S;
        }

        /**
         * @brief An equal operator, checks with equality.
         * @param other the other iterator to compare to.
         * @return `true` if the pointers stored by both iterators are the same.
         */
        bool operator==(const Iterator& other) const {return pointer == other.pointer;}

        /**
         * @brief A non equality operator. Checks that twio iterators are not equal.
         * @param other the iterator to compare to.
         * @return `true` if the pointer addresses stored by both iterators are not the same.
         */
        bool operator!=(const Iterator& other) const { return pointer != other.pointer; }

    private:
        T* pointer;
        uint8_t* maskPointer;
        size_t index;
    };
    MSArray() = default;

    MSArray(std::initializer_list<T> list) : count{list.size()}
    {
        CopyData(std::data(list), list.size()); // 0000 1111
        SetBitsToOne(list.size());

        count = list.size();
    }

    MSArray(const T* ptr, const size_t& ptrSize)
    {
        CopyData(ptr, ptrSize);
        SetBitsToOne(ptrSize);

        count = ptrSize;
    }

    MSArray(MSArray& other)
    {
        Copy(other);
    }

    MSArray(MSArray&& other)
    {
        Copy(other);
        other.count = 0;
    }

    // Operator Overloads

    inline const T& operator[](const size_t& index) const { return Get(index); }

    inline T& operator[](const size_t& index) {
        VerifyIndex(index);
        return Get(index);
    }

    inline MSArray& operator=(const MSArray& other)
    {
        Copy(other);
        return *this;
    }

    inline MSArray& operator=(MSArray&& other)
    {
        Copy(other);
        other.count = 0;
        return *this;
    }

    // Getters

    inline const T& Get(const size_t& index) const
    {
        AssertIsInBoundsAndActive(index);
        return data[index];
    }

    inline T& Get(const size_t& index) { return data[index]; }

    inline const T* Data() const { return data; }
    inline T* Data() { return data; }
    inline constexpr const size_t Size() const { return S; }
    inline const size_t Count() const { return count; }

    // Setters

    inline void Insert(const size_t& index, const T& element)
    {
        VerifyIndex(index);
        Set(index, element);
    }

    inline bool IsInBounds(const size_t& index) const { return index < S; }
    inline bool Empty() const { return count == 0; }

    inline bool Active(const size_t& index) const
    {
        auto byteMask = stateMaskBitfield[index / BYTE_SIZE_IN_BITS];
        return (byteMask & (1 << (index % BYTE_SIZE_IN_BITS))) != 0;
    }

    inline void Remove(const size_t& index)
    {
        AssertIsInBoundsAndActive(index);
        stateMaskBitfield[index / BYTE_SIZE_IN_BITS] &= ~(1 << (index % BYTE_SIZE_IN_BITS));
        count--;
    }

    inline void Clear()
    {
        memset(stateMaskBitfield, 0, (BYTE_MASK_SIZE * GetBitMaskSize()));
        count = 0;
    }

    inline const Iterator begin() const {return Iterator(data, stateMaskBitfield, 0);}
    inline const Iterator end() const {return Iterator(data + S, stateMaskBitfield, S);}

    inline Iterator begin() {return Iterator(data, stateMaskBitfield, 0);}
    inline Iterator end() {return Iterator(data + S, stateMaskBitfield, S);}
private:
    // TODO: Move this to a third intermediate file
    // The number of states we can fit per segment (1 byte = 8 states)
    static inline constexpr uint8_t BYTE_SIZE_IN_BITS = 8;
    // The default object size for each segment (1 byte)
    static inline constexpr uint8_t BYTE_MASK_SIZE = sizeof(uint8_t);
    // Each position represents a byte where all bits before the index are 1.
    // i.e: index 2 = 0000 00111
    static inline constexpr uint8_t MAX_BIT_VALUES[BYTE_SIZE_IN_BITS] = {1, 3, 7, 15, 31, 63, 127, 255};

    constexpr inline size_t GetBitMaskSize() { return (S / BYTE_SIZE_IN_BITS) + 1; }

    inline void Set(const size_t& index, const T& element) { data[index] = element; }

    inline void VerifyIndex(const size_t& index)
    {
        AssertIsInBounds(index);
        count += !Active(index);
        stateMaskBitfield[index / BYTE_SIZE_IN_BITS] |= (1 << (index % BYTE_SIZE_IN_BITS));
    }

    inline void Copy(const MSArray& other)
    {
        CopyData(other.Data(), other.Size());
        CopyMask(other.stateMaskBitfield, GetBitMaskSize());
        count = other.count;
    }

    inline void AssertIsInBoundsAndActive(const size_t& index) const
    {
        AssertIsInBounds(index);
        AssertIsActive(index);
    }

    inline void AssertIsInBounds(const size_t& index) const
    {
        assert(IsInBounds(index) && "The provided index was not in bounds!");
    }

    inline void AssertIsActive(const size_t& index) const
    {
        assert(Active(index) && "Element does not exist in the array!");
    }

    inline void CopyData(const T* src, const size_t& srcSize)
    {
        memcpy(data, src, sizeof(T) * srcSize);
    }

    inline void CopyMask(const uint8_t* source, const size_t maskSize)
    {
        memcpy(stateMaskBitfield, source, BYTE_MASK_SIZE * maskSize);
    }

    inline void SetBitsToOne(const size_t& bits)
    {
        uint8_t bitsToProcess = bits;
        size_t newByteCount = (bitsToProcess / BYTE_SIZE_IN_BITS) + 1;

        for (size_t i = 0; i < newByteCount; i++)
        {
            bool isFullByte = (bitsToProcess - BYTE_SIZE_IN_BITS) >= 0;

            uint8_t maskBits = (bitsToProcess * !isFullByte) + (BYTE_SIZE_IN_BITS * isFullByte);

            stateMaskBitfield[i] =
                MAX_BIT_VALUES[std::clamp<size_t>(maskBits - 1, 0, BYTE_SIZE_IN_BITS)];

            bitsToProcess -= BYTE_SIZE_IN_BITS;
        }
    }

    T data[S];
    uint8_t stateMaskBitfield[(S / BYTE_SIZE_IN_BITS) + 1] {0};
    size_t count {0};
};
}

#endif // SIEGE_ENGINE_STACKARRAY_H
