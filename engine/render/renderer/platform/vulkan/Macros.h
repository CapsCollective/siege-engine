//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MACROS_H
#define SIEGE_ENGINE_MACROS_H

#define GET_RAW(collection, name, count) \
    for (size_t i = 0; i < count; i++) name[i] = collection[i].Str();

#endif // SIEGE_ENGINE_MACROS_H
