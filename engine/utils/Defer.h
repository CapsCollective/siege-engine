//
// Copyright (c) 2023 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DEFER_H
#define SIEGE_ENGINE_DEFER_H

#include <functional>

#include "Macros.h"

namespace Siege
{

/**
 * A basic deferred call object implementation that executes its provided callable on scope exit.
 */
struct ScopedDefer
{
    /**
     * Explicitly deleted zero-param constructor
     */
    ScopedDefer() = delete;

    /**
     * Constructor, specifying owned callable
     * @param callable - the void-returning, parameterless callable to execute
     */
    explicit ScopedDefer(const std::function<void()>& callable) : callable(callable) {}

    /**
     * Destructor, executing callable on scope exit
     */
    ~ScopedDefer()
    {
        callable();
    }

    /**
     * Held void-retuning, parameterless callable
     */
    std::function<void()> callable;
};

#define _DEFER_VAR_NAME CONCAT_SYMBOL(_defer_, __COUNTER__)
#define defer(callable) auto _DEFER_VAR_NAME = Siege::ScopedDefer(callable)

} // namespace Siege

#endif // SIEGE_ENGINE_DEFER_H
