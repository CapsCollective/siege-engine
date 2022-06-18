//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SERVICELOCATORUTILS_H
#define SIEGE_ENGINE_SERVICELOCATORUTILS_H

#include "Macros.h"

#define MAKE_GETTER(name) CONCAT_SYMBOL(Get, name)
#define REGISTER_SERVICE(name, type)   \
    static type*& MAKE_GETTER(name)()  \
    {                                  \
        static type* service;          \
        return service;                \
    }                                  \
                                       \
    static void Provide(type* service) \
    {                                  \
        MAKE_GETTER(name)() = service; \
    }

#endif // SIEGE_ENGINE_SERVICELOCATORUTILS_H
