#pragma once

#include <memory>
#include <iostream>
#include <cstdint>

namespace SnekVk::Utils
{
    template<typename Array>
    class ArrayIterator
    {
        public: 
            using ValueType = typename Array::ValueType;
            using PointerType = ValueType*;
            using ReferenceType = ValueType&;
        public:

        ArrayIterator(PointerType pointer) 
            : pointer{pointer} {};
        
        ArrayIterator& operator++() 
        {
            pointer++;
            return *this;
        }

        ArrayIterator operator++(int)
        {
            ArrayIterator iterator = *this;
            ++(*this);
            return iterator;
        }

        ArrayIterator& operator--() 
        {
            pointer--;
            return *this;
        }

        ArrayIterator operator--(int)
        {
            ArrayIterator iterator = *this;
            --(*this);
            return iterator;
        }

        ReferenceType operator[](size_t index)
        {
            return *(pointer + index);
        }

        PointerType operator->()
        {
            return pointer;
        }

        ReferenceType operator*()
        {
            return *pointer;
        }

        bool operator==(const ArrayIterator& other) const
        {
            return pointer == other.pointer;
        }

        bool operator!=(const ArrayIterator& other) const
        {
            return !(*this == other);
        }

        private:
        
        PointerType pointer;
    };

    template<typename T>
    class Array 
    {
        public:
        using ValueType = T;
        using Iterator = ArrayIterator<Array>;
        public:

        Array() : size{0} {};

        Array(size_t size) : size{size} 
        {
            data = new T[size];
        }

        Array(uint32_t size) : size{static_cast<size_t>(size)} 
        {
            data = new T[this->size];
        }

        Array(int size) : size{static_cast<size_t>(size)} 
        {
            data = new T[this->size];
        }

        Array(std::initializer_list<T> values)
        {
            data = new T[values.size()];
            
            size_t index = 0;
            for (auto val : values)
            {
                data[index] = val;
                index++;
            }

            size = values.size();
        }

        Array(const Array<T>& other)
        {
            if (data) delete [] data;

            Copy(other);
        }

        void operator=(const Array<T>& other)
        {
            if (data) delete [] data;

            Copy(other);
        }

        void operator=(Array<T>&& other)
        {
            if (data) delete [] data;

            data = std::move(other.data);
            other.data = nullptr;

            size = std::move(other.Size());
        }

        ~Array() { 
            if (data) delete [] data;
        }

        const size_t Size() const { return size; }
        void SetSize(size_t size) { this->size = size; }

        T& operator[] (size_t index) { return data[index]; }
        const T& operator[] (size_t index) const { return data[index]; }

        T* Data() { return data; }

        Iterator begin() { return Iterator(data); }
        Iterator end() { return Iterator(data + size); }
        
        const Iterator begin() const { return Iterator(data); }
        const Iterator end() const { return Iterator(data + size); }

        private:

        size_t size;
        T* data = nullptr;

        void Copy(const Array<T>& other) 
        {
            data = new T[other.Size()];
            size = other.Size();

            size_t index = 0; 
            for (auto value : other)
            {
                data[index] = value;
                index++;
            }
        }      
    };
}