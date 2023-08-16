# Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib
#

param ([String] $Path)

if (!(Test-Path $Path)) { New-Item -Path $Path -ItemType directory }