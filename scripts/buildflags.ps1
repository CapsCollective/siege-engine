# Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

[string[]] $Dirs = @()
[string] $CXXFlags = @()
[string] $Target = @()

[string] $current_arg = @()

foreach ($arg in $args)
{
    [string] $val = $arg

    [string] $val = $arg
    if ($val.Contains("--")) {
        if ($current_arg -ne $val) { $current_arg = $val }
        continue
    }

    if ($current_arg -eq "--Target") { $Target += $val }
    if ($current_arg -eq "--CXXFlags") { $CXXFlags += ' ' + $val }
    if ($current_arg -eq "--Dirs") { $Dirs += $val }
}

[string] $Contents = @()

if (Test-Path $Target) { $Contents = Get-Content -Path $Target -Raw }

if ($Contents.Trim() -ne $CXXFlags.Trim())
{
    foreach($i in $Dirs) { if (Test-Path $i) { Remove-Item -Path $i -Recurse } }
    Set-Content -Path $Target -Value $CXXFlags.Trim()
}