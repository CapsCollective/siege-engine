#pragma once

#include "Array.h"
#include <memory>
#include <iostream>
#include <cstdint>
#include <cassert>

namespace SnekVk::Utils
{
    template<typename T, size_t S>
    
    class StackArray
    {
        public:
        using ValueType = T;
        using Iterator = ArrayIterator<StackArray>;

        public: 

        StackArray() { memset(states, false, sizeof(bool) * S); }

        StackArray(std::initializer_list<T> values)
        {
            for(auto& value : values) Append(value);
        }

        StackArray(const StackArray<T, S>& other)
        {
            for(auto& value : other) Append(value);
        }

        ~StackArray() {}

        size_t Count() { return count; }
        size_t Size() { return S; }
        
        void Set(size_t index, T value)
        {
            count += !Exists(index);

            states[index] = true; 
            data[index] = value;
        }

        void Append(T value)
        {
            assert(count < S 
                    && std::string("Too many elements added to array. Max is ")
                    .append(std::to_string(S)).c_str());

            Set(count, value);
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

        T& operator[] (size_t index) { return Get(index); }

        const T& operator[] (size_t index) const { return Get(index); }

        Iterator begin() { return Iterator(data); }
        Iterator end() { return Iterator(data + count); }

        const Iterator begin() const { return Iterator(data); }
        const Iterator end() const { return Iterator(data + count); }

        private:

        bool isValidIndex(size_t index)
        {
            return index < S;
        }

        bool Exists(size_t index)
        {
            assert(isValidIndex(index) && "Error: index is out of bounds!");

            return states[index];
        }

        bool states[S];
        T data[S];
        size_t count {0}; 
    };
}