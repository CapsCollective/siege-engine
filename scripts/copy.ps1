# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib
#

param( [String] $Source, [String] $Destination, [String] $Filter)

Get-ChildItem $Source | Copy-Item -Filter $Filter -Destination $Destination -Recurse -Force