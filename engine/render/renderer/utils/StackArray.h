//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_STACK_ARRAY_H
#define SIEGE_ENGINE_STACK_ARRAY_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>

#include "Array.h"

namespace Siege::Utils
{
template<typename T, size_t S>
class StackArray
{
public:

    using ValueType = T;
    using Iterator = ArrayIterator<StackArray<T, S>>;

public:

    StackArray()
    {
        memset(states, false, sizeof(bool) * S);
    }

    StackArray(std::initializer_list<T> values)
    {
        for (auto& value : values) Append(value);
    }

    StackArray(StackArray<T, S>& other)
    {
        for (auto& value : other) Append(value);
    }

    ~StackArray() {}

    size_t Count()
    {
        return count;
    }

    size_t Count() const
    {
        return count;
    }

    size_t Size()
    {
        return S;
    }
    T* Data()
    {
        return data;
    }

    const T* Data() const
    {
        return data;
    }

    void Set(size_t index, T value)
    {
        count += !Exists(index);

        states[index] = true;
        data[index] = value;
    }

    void Append(T value)
    {
        assert(count < S && std::string("Too many elements added to array. Max is ")
                                .append(std::to_string(S))
                                .c_str());
        Activate(count);
        Set(count - 1, value);
    }

    void Remove(size_t index)
    {
        if (!Exists(index)) return;

        states[index] = false;
        count--;
    }

    T& Get(size_t index)
    {
        assert(Exists(index) && "Error: trying to access empty element");

        return data[index];
    }

    T& operator[](size_t index)
    {
        return Get(index);
    }

    const T& operator[](size_t index) const
    {
        return Get(index);
    }

    Iterator begin()
    {
        return Iterator(&data[0]);
    }
    Iterator end()
    {
        return Iterator(&data[count]);
    }

    const Iterator begin() const
    {
        return Iterator(&data[0]);
    }
    const Iterator end() const
    {
        return Iterator(&data[count]);
    }

    bool Exists(size_t index)
    {
        assert(isValidIndex(index) && "Error: index is out of bounds!");

        return states[index];
    }

    void Activate(size_t index)
    {
        assert(isValidIndex(index) && "Error: index is out of bounds!");

        states[index] = true;

        count++;
    }

    void Allocate(size_t slots)
    {
        assert(slots < S && count + slots < S &&
               "Error: trying to allocate more slots than actually exist");

        for (size_t i = 0; i < slots; i++)
        {
            Activate(i);
        }
    }

    void Allocate(size_t slots, T defaultValue)
    {
        assert(slots < S && count + slots < S &&
               "Error: trying to allocate more slots than actually exist");

        for (size_t i = 0; i < slots; i++)
        {
            Set(i, defaultValue);
        }
    }

    void Clear()
    {
        count = 0;
        memset(states, false, sizeof(bool) * S);
    }

private:

    bool isValidIndex(size_t index)
    {
        return index < S;
    }

    bool states[S];
    T data[S];
    size_t count {0};
};
} // namespace Siege::Utils

#endif