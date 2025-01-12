# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib
#

[string] $PkgName = @()
[string] $ExeName = @()
[string] $OutputDir = @()
[string] $BuildDir = @()
[string[]] $EnvVars = @()
[string[]] $Excludes = @()

[string] $current_arg = @()

foreach ($arg in $args)
{

    [string] $val = $arg
    if ($val.Contains("--")) {
        if ($current_arg -ne $val)
        {
            $current_arg = $val
        }
        continue
    }

    if ($current_arg -eq "--pkg-name") { $PkgName += $val.trim("\n") + " "}
    if ($current_arg -eq "--exe-name") { $ExeName += $val }
    if ($current_arg -eq "--output-dir") { $OutputDir += $val }
    if ($current_arg -eq "--build-dir") { $BuildDir += $val }
    if ($current_arg -eq "--env-vars") { $EnvVars += $val }
    if ($current_arg -eq "--excludes") { $Excludes += $val }
}

$PkgName = $PkgName.TrimEnd()
[string] $PkgFile = "$OutputDir/$PkgName"

Write-Output "PACKAGING: $args"

if (Test-Path $OutputDir/$PkgName) { Remove-Item -Path $OutputDir/$PkgName -Recurse }

New-Item -Path "$PkgFile" -ItemType directory -Force

if (Test-Path -Path $BuildDir/lib)
{
    Get-ChildItem -Directory $BuildDir/lib | Copy-Item -Filter * -Destination $PkgFile -Recurse -Force
}

Get-ChildItem $BuildDir | Copy-Item -Filter **.exe -Destination $PkgFile -Recurse -Force

[string] $val

if ($Excludes.Count -gt 0)
{
    foreach ($i in $Excludes) { if ($1 -ne $OutputDir) { Remove-Item -Path $PkgFile/$i -Recurse  -Force -ErrorAction SilentlyContinue } }
}

if ((Test-Path -Path "$PkgFile/lib") -and (-not (Test-Path -Path "$PkgFile/lib/*")))
{
    Remove-Item -Path $PkgFile/lib -Force
}