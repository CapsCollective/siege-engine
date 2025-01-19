#
# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

param( [String] $LibSourceDir, [String] $LibDestDir )

robocopy $LibSourceDir $LibDestDir /e /im /is /it

if (Test-Path -Path "$LibSourceDir/lib/*.dll")
{
    robocopy $LibDestDir/lib LibDestDir *.dll /mov /im /is /it
}

if (Test-Path -Path "$LibDestDir/lib/*")
{
    Remove-Item -Path "$LibDestDir/lib" -Force
}
