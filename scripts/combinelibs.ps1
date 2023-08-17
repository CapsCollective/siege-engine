# Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

[string[]] $targets = "*.obj", "*.o"

function Make-Dir {
    param ( [string] $Dir_Path )
    if (!(Test-Path $Dir_Path)) { New-Item -Path $Dir_Path -ItemType directory }
}

function Get-Files {
    param ([string] $Path=@(), [string[]] $Extensions=@())
    $items = Get-ChildItem -Path $Path/* -Include $Extensions -Recurse
    $items
    return
}

function Append-To-Lib {
    param ([string] $Dst_Lib=@(), [string[]] $Src_Files=@())
    ar -q "$Dst_Lib" $Src_Files
}

function Extract-Lib {
    param ([string] $Src_Lib=@(), [string] $Output_Dir=@(), [string] $Lib_Name=@())

    ar -x $Src_Lib --output $Output_Dir

    Append-To-Lib "$Output_Dir/lib$Lib_Name.a" (Get-Files -path $Output_Dir -extensions $targets)
    Remove-Item -Path $Output_Dir/* -Include $targets
}

[string[]] $libs = @()
[string[]] $objs = @()
[string] $output_dir = @()
[string] $output_name = @()

[string] $current_arg = @()

foreach($arg in $args)
{
    [string] $val = $arg
    if ($val.Contains("--")) {
        if ($current_arg -ne $val) { $current_arg = $val }
        continue
    }

    if ($current_arg -eq "--src_libs") { $libs += $val }
    elseif ($current_arg -eq "--src_objs") { $objs += $val }
    elseif ($current_arg -eq "--output_name") { $output_name = $val }
    elseif ($current_arg -eq "--output_dir") { $output_dir = $val }
}

Make-Dir $output_dir

ar -crs "$output_dir/lib$output_name.a" $objs

foreach($src in $libs) { Extract-Lib -Src_Lib $src -Output_Dir $output_dir -Lib_Name $output_name }