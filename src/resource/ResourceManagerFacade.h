#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGERFACADE_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGERFACADE_H

#include "../utils/String.h"

class ResourceManagerFacade
{
public:

    static void FreeResources();

    static void SetBaseDirectory(const String& dir);

    static const String& GetBaseDirectory();
};

#endif // A_DARK_DISCOMFORT_RESOURCEMANAGERFACADE_H
