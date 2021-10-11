#pragma once

#include <memory>
#include <iostream>
#include <cstdint>
#include <cassert>

namespace SnekVk::Utils
{
    template<typename T, size_t S>
    
    // TODO: Implement iterator for this array
    class StackArray
    {
        public: 

        StackArray() 
        {
            // Set all values to nullptr to start with
            memset(data, 0, sizeof(T) * S);
            memset(states, false, sizeof(bool) * S);
        }

        StackArray(std::initializer_list<T> values)
        {
            size_t index = 0; 
            for(auto& value : values)
            {
                Set(index, value);
                index++;
            }
        }

        // TODO: Implement copy constructor
        StackArray(StackArray& other);

        // TODO: Implement move constructor
        StackArray(StackArray&& other);

        ~StackArray() {}

        size_t Count() { return count; }
        size_t Size() { return S; }
        
        void Set(size_t index, T value)
        {
            if (!Exists(index)) 
            {
                count++;
                states[index] = true;
            }

            data[index] = value;
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