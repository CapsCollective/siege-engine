#ifndef A_DARK_DISCOMFORT_SERVICELOCATORUTILS_H
#define A_DARK_DISCOMFORT_SERVICELOCATORUTILS_H

#include "Macros.h"

#define MAKE_GETTER(name) CONCAT_SYMBOL(Get, name)
#define REGISTER_SERVICE(name, type) \
    static type*& MAKE_GETTER(name)() \
    { \
        static type* service; \
        return service; \
    } \
    \
    static void Provide(type* service) \
    { \
        MAKE_GETTER(name)() = service; \
    }

#endif //A_DARK_DISCOMFORT_SERVICELOCATORUTILS_H
