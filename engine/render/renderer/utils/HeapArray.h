//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_HEAP_ARRAY_H
#define SIEGE_ENGINE_HEAP_ARRAY_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <utility>

typedef const size_t sizet;

namespace Siege
{
template<typename T>
class HeapArray
{
public:

    class Iterator
    {
    public:

        explicit Iterator(T* valuePointer) : pointer {valuePointer} {}

        Iterator& operator++()
        {
            pointer++;
            return *this;
        }

        const Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++(*this);
            return iterator;
        }

        Iterator& operator--()
        {
            pointer--;
            return *this;
        }

        const Iterator operator--(int)
        {
            Iterator iterator = *this;
            --(*this);
            return iterator;
        }

        T& operator[](size_t index)
        {
            return *(pointer + index);
        }

        T* operator->()
        {
            return pointer;
        }

        T& operator*()
        {
            return *pointer;
        }

        bool operator==(const Iterator& other) const
        {
            return pointer == other.pointer;
        }

        bool operator!=(const Iterator& other) const
        {
            return pointer != other.pointer;
        }

    private:

        T* pointer;
    };

    HeapArray() : HeapArray(0, 0, 0, nullptr, nullptr) {}

    explicit HeapArray(const size_t& arraySize) :
        HeapArray(arraySize, 0, ((arraySize / SEGMENT_SIZE) + 1))
    {}

    HeapArray(const std::initializer_list<T>& list) : HeapArray(list.size())
    {
        for (auto& element : list) Insert(count, element);
    }

    HeapArray(const HeapArray<T>& other) : size {other.size}, count {other.count}, segments {other.segments}
    {
        if (other.size == 0 && other.data == nullptr) return;

        AllocateMemory(size, segments);

        CopyData(other.data, sizeof(T) * size);
        CopyMasks(other.stateMasks, SEGMENT_OBJ_SIZE * segments);

        count = other.Count();
    }

    HeapArray(HeapArray<T>&& other) noexcept :
        size {std::move(other.size)},
        count {std::move(other.count)},
        segments {std::move(other.segments)},
        stateMasks {std::move(other.stateMasks)},
        data {std::move(other.data)}
    {
        other.ResetValues();
    }

    ~HeapArray()
    {
        Destroy();
    }

    void Destroy()
    {
        DeallocateMemory();
        ResetValues();
    }

    const T& operator[](const size_t& index) const
    {
        return Get(index);
    }

    T& operator[](const size_t& index)
    {
        VerifyIndex(index);
        return Get(index);
    }

    HeapArray<T>& operator=(const HeapArray<T>& other)
    {
        if (this == &other) return *this;

        Destroy();

        Copy(other);

        return *this;
    }

    HeapArray<T>& operator=(HeapArray<T>&& other) noexcept
    {
        if (this == &other) return *this;

        Destroy();

        data = std::move(other.data);
        stateMasks = std::move(other.stateMasks);
        size = std::move(other.size);
        count = std::move(other.count);
        segments = std::move(other.segments);

        other.ResetValues();

        return *this;
    }

    const T& Get(const size_t& index) const
    {
        CheckIfActive(index);

        return data[index];
    }

    T& Get(const size_t& index)
    {
        CheckIfActive(index);

        return data[index];
    }

    void Insert(const size_t index, const T& element)
    {
        VerifyIndex(index);
        Set(index, element);
    }

    void Remove(const size_t& index)
    {
        CheckIfIndexInBounds(index);
        stateMasks[index / SEGMENT_SIZE] &= ~(1 << (index % SEGMENT_SIZE));
        count--;
    }

    bool IsInBounds(const size_t& index)
    {
        return index < size;
    }

    void Resize(const size_t& newSize)
    {
        T tempArray[size];
        uint8_t tempMasks[segments];

        size_t tempCount = count;
        size_t tempSize = size;
        size_t tempSegments = segments;

        memcpy(tempArray, data, sizeof(T) * size);
        memcpy(tempMasks, stateMasks, SEGMENT_OBJ_SIZE * segments);

        Destroy();

        AllocateMemory(newSize, ((newSize / SEGMENT_SIZE) + 1));

        CopyData(tempArray, sizeof(T) * tempSize);
        CopyMasks(tempMasks, SEGMENT_OBJ_SIZE * tempSegments);

        SetSizes(newSize, ((newSize / SEGMENT_SIZE) + 1));

        count = tempCount;
    }

    bool Active(const size_t& index)
    {
        auto segment = stateMasks[index / SEGMENT_SIZE];

        return (segment & (1 << (index % SEGMENT_SIZE))) != 0;
    }

    void Clear()
    {
        count = 0;
        memset(stateMasks, false, (SEGMENT_OBJ_SIZE * SEGMENT_SIZE));
    }

    const size_t& Count() const
    {
        return count;
    }

    const size_t& Size() const
    {
        return size;
    }

    const T* Data() const
    {
        return data;
    }

    T* Data()
    {
        return data;
    }

    Iterator begin()
    {
        return Iterator(data);
    }
    Iterator end()
    {
        return Iterator(data + size);
    }

    const Iterator begin() const
    {
        return Iterator(data);
    }
    const Iterator end() const
    {
        return Iterator(data + size);
    }

private:

    // Class constants

    static constexpr uint8_t SEGMENT_SIZE = 8;
    static constexpr uint8_t SEGMENT_OBJ_SIZE = sizeof(uint8_t);

    // Private Constructors

    HeapArray(sizet& newSize, sizet& newCount, sizet& newSegments, uint8_t* masks, T* data) :
        size {newSize},
        count {newCount},
        segments {newSegments},
        stateMasks {masks},
        data {data}
    {}

    HeapArray(sizet& newSize, sizet& newCount, sizet& masksSize) :
        HeapArray(newSize, newCount, masksSize, new uint8_t[masksSize], new T[newSize])
    {}

    // Functions

    void Set(const size_t& index, const T& value)
    {
        data[index] = value;
    }

    void VerifyIndex(const size_t& index)
    {
        CheckIfIndexInBounds(index);
        count += !Active(index);
        stateMasks[index / SEGMENT_SIZE] |= (1 << (index % SEGMENT_SIZE));
    }

    void CheckIfIndexInBounds(const size_t& index)
    {
        assert(IsInBounds(index) && "The index provided is larger than the size of the array!");
    }

    void CheckIfActive(const size_t& index)
    {
        assert(Active(index) && "Element does not exist in the array!");
    }

    void SetSizes(const size_t& arraySize, const size_t& segmentCount)
    {
        size = arraySize;
        segments = segmentCount;
    }

    void AllocateMemory(const size_t& arraySize, const size_t& segmentCount)
    {
        data = new T[arraySize];
        stateMasks = new uint8_t[segmentCount];
    }

    void CopyData(const T* source, const size_t& size)
    {
        memcpy(data, source, size);
    }

    void CopyMasks(const uint8_t * source, const size_t& size)
    {
        memcpy(stateMasks, source, size);
    }

    void DeallocateMemory()
    {
        delete[] data;
        delete[] stateMasks;
    }

    void ResetValues()
    {
        data = nullptr;
        stateMasks = nullptr;

        size = 0;
        count = 0;
        segments = 0;
    }

    void Copy(const HeapArray<T>& other)
    {
        if (other.size == 0 && other.data == nullptr) return;

        count = other.count;

        SetSizes(other.size, other.segments);
        AllocateMemory(size, segments);

        CopyData(other.data, sizeof(T) * size);
        CopyMasks(other.stateMasks, SEGMENT_OBJ_SIZE * segments);
    }

    // Tracker Variables
    size_t size {};
    size_t count {};

    // TODO(Aryeh): We could probably remove this. Not sure if we want to cache this value.
    size_t segments {};

    // Pointers
    uint8_t* stateMasks {};
    T* data;
};

} // namespace Siege

#endif // SIEGE_ENGINE_HEAP_ARRAY_H
