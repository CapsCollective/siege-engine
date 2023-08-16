//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_STATICS_H
#define SIEGE_ENGINE_STATICS_H

struct FT_LibraryRec_;
typedef struct FT_LibraryRec_* FontLib;

namespace Siege
{
/**
 * Standalone class for managing all statics required by the application. This is meant to store and
 * manage required libraries (such as freetype) which are generally stored as a singleton.
 */
class Statics
{
private:

public:

    // Getters

    /**
     * Get the font lib used to create the glyphs for the application
     * @return the stored font library
     */
    static inline FontLib& GetFontLib()
    {
        return freetype;
    }

private:

    friend class Renderer;

    /**
     * Initialises the statics stored by the class
     */
    static void Initialise();

    /**
     * Frees all the statics stored by the class
     */
    static void Free();
    static FontLib freetype;
};
} // namespace Siege

#endif // SIEGE_ENGINE_STATICS_H
