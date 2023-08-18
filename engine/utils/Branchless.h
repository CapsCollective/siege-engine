//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BRANCHLESS_H
#define SIEGE_ENGINE_BRANCHLESS_H

// Branchless conditional assignment macros
#define IF_ASSIGN(expr, if, else) ((expr) * if) + (!(expr) * else)

#define IF(...) IF_ASSIGN(__VA_ARGS__)

#define THEN ,
#define ELSE THEN

#endif // SIEGE_ENGINE_BRANCHLESS_H
