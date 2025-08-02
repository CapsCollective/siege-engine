//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BINARYSERIALISATION_H
#define SIEGE_ENGINE_BINARYSERIALISATION_H

#include <map>
#include <set>
#include <vector>

#include "String.h"

namespace Siege::BinarySerialisation
{

enum SerialisationMode : u_int8_t
{
    SERIALISE,
    DESERIALISE,
};

#define SERIALISE_NATIVE(type)                                                \
    inline void serialise(Buffer& buffer, type& value, SerialisationMode mode)       \
    {                                                                         \
        serialiseNative(buffer, value, mode);                                 \
    }                                                                         \
    inline void serialise(Buffer& buffer, const type& value, SerialisationMode mode) \
    {                                                                         \
        serialiseNative(buffer, value, mode);                                 \
    }

struct Buffer
{
    std::basic_string<uint8_t> data;
    uint32_t cursor = 0;

    void Reset()
    {
        data.clear();
        cursor = 0;
    }
};

struct BinarySerialisable
{
    virtual ~BinarySerialisable() = default;

    virtual void serialise(Buffer& buffer, SerialisationMode mode) {}
};

template<typename T>
void serialiseNative(Buffer& buffer, T& value, SerialisationMode mode)
{
    switch (mode)
    {
        case SERIALISE:
        {
            size_t was = buffer.data.size();
            buffer.data.resize(was + sizeof(value));
            *(T*) &buffer.data[was] = value;
            break;
        }
        case DESERIALISE:
        {
            value = *(const T*) (buffer.data.data() + buffer.cursor);
            buffer.cursor += sizeof(value);
            break;
        }
    }
}

template<typename T>
void serialiseNative(Buffer& buffer, const T& value, SerialisationMode mode)
{
    T& nonConstValue = const_cast<T&>(value);
    serialiseNative(buffer, nonConstValue, mode);
}

SERIALISE_NATIVE(bool)
SERIALISE_NATIVE(char)
SERIALISE_NATIVE(unsigned char)
SERIALISE_NATIVE(uint32_t)
SERIALISE_NATIVE(int32_t)
SERIALISE_NATIVE(float)
SERIALISE_NATIVE(double)

template<typename T1, typename T2>
void serialise(Buffer& buffer, std::pair<T1, T2>& value, SerialisationMode mode)
{
    serialise(buffer, value.first, mode);
    serialise(buffer, value.second, mode);
}

template<typename T>
void serialiseContainer(Buffer& buffer, T& value)
{
    uint32_t size = value.size();
    serialise(buffer, size, SERIALISE);
    for (auto& entry : value)
    {
        serialise(buffer, entry, SERIALISE);
    }
}

inline void serialise(Buffer& buffer, String& value, SerialisationMode mode)
{

    switch (mode)
    {
        case SERIALISE:
        {
            std::string temp(value.Str());
            serialiseContainer(buffer, temp);
            break;
        }
        case DESERIALISE:
        {
            value.Clear();

            uint32_t size;
            serialise(buffer, size, mode);
            value.Reserve(size);

            value.Append(reinterpret_cast<char*>(buffer.data.data() + buffer.cursor), size);
            buffer.cursor += size;
            break;
        }
    }
}

template<typename T>
void serialise(Buffer& buffer, std::vector<T>& value, SerialisationMode mode)
{
    switch (mode)
    {
        case SERIALISE:
        {
            serialiseContainer(buffer, value);
            break;
        }
        case DESERIALISE:
        {
            value.clear();

            uint32_t size;
            serialise(buffer, size, mode);
            value.reserve(size);

            T entry {};
            while (size--)
            {
                serialise(buffer, entry, mode);
                value.push_back(entry);
            }
            break;
        }
    }
}

template<typename T>
void serialise(Buffer& buffer, std::set<T>& value, SerialisationMode mode)
{
    switch (mode)
    {
        case SERIALISE:
        {
            serialiseContainer(buffer, value);
            break;
        }
        case DESERIALISE:
        {
            value.clear();

            uint32_t size;
            serialise(buffer, size, mode);

            T entry {};
            while (size--)
            {
                serialise(buffer, entry, mode);
                value.insert(entry);
            }
            break;
        }
    }
}

template<typename T1, typename T2>
void serialise(Buffer& buffer, std::map<T1, T2>& value, SerialisationMode mode)
{
    switch (mode)
    {
        case SERIALISE:
        {
            serialiseContainer(buffer, value);
            break;
        }
        case DESERIALISE:
        {
            value.clear();

            uint32_t size;
            serialise(buffer, size, mode);

            std::pair<T1, T2> entry;
            while (size--)
            {
                serialise(buffer, entry, mode);
                value.insert(entry);
            }
            break;
        }
    }
}

template<typename T>
void serialise(Buffer& buffer, T& value, SerialisationMode mode)
{
    value.serialise(buffer, mode);
}

} // namespace Siege::BinarySerialisation

#endif // SIEGE_ENGINE_BINARYSERIALISATION_H
