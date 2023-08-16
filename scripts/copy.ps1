# Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib
#

param( [String] $Source, [String] $Destination, [String] $Filter)

Get-ChildItem $Source | Copy-Item -Filter $Filter -Destination $Destination -Recurse -Force