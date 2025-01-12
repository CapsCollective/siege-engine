# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib
#

param ([String] $Path)

if (-not(Test-Path $Path)) { New-Item -Path $Path -ItemType "directory" -Force }