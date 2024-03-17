//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RESOURCESYSTEM_H
#define SIEGE_ENGINE_RESOURCESYSTEM_H

#include <utils/String.h>

namespace Siege
{

class ResourceSystem
{
public:

    // Public methods

    static ResourceSystem& GetInstance();

    bool MountPackFile(const String& searchPath = {});

    void UnmountPackFile();

    class PackFile* GetPackFile();

private:

    // Private fields

    PackFile* packFile = nullptr;
};

} // namespace Siege

#endif // SIEGE_ENGINE_RESOURCESYSTEM_H
