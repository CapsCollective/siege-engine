# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

if ($Args.Count -ne 1) {
    Write-Output "Requires one argument (e.g. applyenv <env_file>)"
    exit 1
}

$files = Get-Content -Path $Args[0]

$files | ForEach-Object {

    $variable=$_ -split "="

    $env_name = $variable[0]
    $env_value = $variable[1]

    Set-Item -Path env:$env_name -Value "$env_value"
}