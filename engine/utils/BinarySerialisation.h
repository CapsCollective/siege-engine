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

#include "Colour.h"
#include "String.h"
#include "math/vec/Vec.h"
#include "math/vec/Vec2.h"
#include "math/vec/Vec3.h"

namespace Siege::BinarySerialisation
{

enum SerialisationMode : u_int8_t
{
    SERIALISE,
    DESERIALISE,
};

#define SERIALISE_NATIVE(type)                                                       \
    inline void serialise(Buffer& buffer, type& value, SerialisationMode mode)       \
    {                                                                                \
        serialiseNative(buffer, value, mode);                                        \
    }                                                                                \
    inline void serialise(Buffer& buffer, const type& value, SerialisationMode mode) \
    {                                                                                \
        serialiseNative(buffer, value, mode);                                        \
    }

struct Buffer
{
    std::basic_string<uint8_t> data;
    uint32_t cursor = 0;

    void Fill(const uint8_t* sourceData, uint32_t size)
    {
        data.assign(sourceData, size);
    }

    void Reset()
    {
        data.clear();
        cursor = 0;
    }
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

inline void serialise(Buffer& buffer, Vec2& value, SerialisationMode mode)
{
    serialiseNative<float>(buffer, value.x, mode);
    serialiseNative<float>(buffer, value.y, mode);
}

inline void serialise(Buffer& buffer, Vec3& value, SerialisationMode mode)
{
    serialiseNative<float>(buffer, value.x, mode);
    serialiseNative<float>(buffer, value.y, mode);
    serialiseNative<float>(buffer, value.z, mode);
}

inline void serialise(Buffer& buffer, FColour& value, SerialisationMode mode)
{
    serialiseNative<float>(buffer, value.r, mode);
    serialiseNative<float>(buffer, value.g, mode);
    serialiseNative<float>(buffer, value.b, mode);
    serialiseNative<float>(buffer, value.a, mode);
}

inline void serialise(Buffer& buffer, String& value, SerialisationMode mode)
{
    switch (mode)
    {
        case SERIALISE:
        {
            uint32_t size = value.Size() + 1;
            serialise(buffer, size, mode);

            size_t was = buffer.data.size();
            buffer.data.resize(was + size);

            memcpy(&buffer.data[was], value.Str(), size);
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

} // namespace Siege::BinarySerialisation

#endif // SIEGE_ENGINE_BINARYSERIALISATION_H
