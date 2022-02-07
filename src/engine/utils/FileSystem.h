#ifndef A_DARK_DISCOMFORT_FILESYSTEM_H
#define A_DARK_DISCOMFORT_FILESYSTEM_H

#include "String.h"

namespace FileSystem
{

String Read(const String& filename);

bool Exists(const String& filename);

bool Save(const String& filename, const String& content);

} // namespace FileSystem

#endif // A_DARK_DISCOMFORT_FILESYSTEM_H
