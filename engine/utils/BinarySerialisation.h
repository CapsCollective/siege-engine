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

#include <vector>
#include <set>
#include <map>

#include "String.h"

namespace Siege::BinarySerialisation
{

#define SERIALISE_NATIVE(type) \
    void serialise(Buffer& buffer, type& value, bool deserialise = false) { \
        serialiseNative(buffer, value, deserialise); } \
    void serialise(Buffer& buffer, const type& value, bool deserialise = false) { \
        serialiseNative(buffer, value, deserialise); }

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

    virtual void serialise(Buffer& buffer, bool deserialise) {}
};

template<typename T>
void serialiseNative(Buffer& buffer, T& value, bool deserialise = false)
{
    if (deserialise)
    {
        value = *(const T*)(buffer.data.data() + buffer.cursor);
        buffer.cursor += sizeof(value);
    }
    else
    {
        size_t was = buffer.data.size();
        buffer.data.resize(was + sizeof(value));
        *(T*)&buffer.data[was] = value;
    }
}

template<typename T>
void serialiseNative(Buffer& buffer, const T& value, bool deserialise = false)
{
    T& nonConstValue = const_cast<T&>(value);
    serialiseNative(buffer, nonConstValue, deserialise);
}

SERIALISE_NATIVE(bool)
SERIALISE_NATIVE(uint32_t)
SERIALISE_NATIVE(int32_t)
SERIALISE_NATIVE(float)
SERIALISE_NATIVE(double)

template<typename T1, typename T2>
void serialise(Buffer& buffer, std::pair<T1, T2>& value, bool deserialise = false)
{
    serialise(buffer, value.first, deserialise);
    serialise(buffer, value.second, deserialise);
}

template<typename T>
void serialiseContainer(Buffer& buffer, T& value)
{
    uint32_t size = value.size();
    serialise(buffer, size);
    for (auto& entry : value)
    {
        serialise(buffer, entry);
    }
}

template<typename T>
void serialise(Buffer& buffer, std::vector<T>& value, bool deserialise = false)
{
    if (deserialise)
    {
        value.clear();

        uint32_t size;
        serialise(buffer, size, true);
        value.reserve(size);

        T entry {};
        while (size--)
        {
            serialise(buffer, entry, true);
            value.push_back(entry);
        }
    }
    else serialiseContainer(buffer, value);
}

template<typename T>
void serialise(Buffer& buffer, std::set<T>& value, bool deserialise = false)
{
    if (deserialise)
    {
        value.clear();

        uint32_t size;
        serialise(buffer, size, true);

        T entry {};
        while (size--)
        {
            serialise(buffer, entry, true);
            value.insert(entry);
        }
    }
    else serialiseContainer(buffer, value);
}

template<typename T1, typename T2>
void serialise(Buffer& buffer, std::map<T1, T2>& value, bool deserialise = false)
{
    if (deserialise)
    {
        value.clear();

        uint32_t size;
        serialise(buffer, size, true);

        std::pair<T1, T2> entry;
        while (size--)
        {
            serialise(buffer, entry, true);
            value.insert(entry);
        }
    }
    else serialiseContainer(buffer, value);
}

template<typename T>
void serialise(Buffer& buffer, T& value, bool deserialise = false)
{
    value.serialise(buffer, deserialise);
}

} // namespace Siege::BinarySerialisation

#endif // SIEGE_ENGINE_BINARYSERIALISATION_H
