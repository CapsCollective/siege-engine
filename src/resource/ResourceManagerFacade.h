#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGERFACADE_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGERFACADE_H

#include <string>

class ResourceManagerFacade
{
public:

    static void FreeResources();

    static void SetBaseDirectory(const std::string& dir);

    static const std::string& GetBaseDirectory();
};

#endif // A_DARK_DISCOMFORT_RESOURCEMANAGERFACADE_H
