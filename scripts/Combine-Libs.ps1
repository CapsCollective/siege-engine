param (
    [string[]] $Src_Libs=@(),
    [string[]] $Src_Objs=@(),
    [string] $Extraction_Dir=@(),
    [string] $Output_Name=@()
)

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
    ar -rcs "$Dst_Lib.a" $Src_Files
}

function Extract-Lib {
    param ([string] $Src_Lib=@())
    ar -x $Src_Lib --output $Extraction_Dir

    Append-To-Lib $Extraction_Dir/$Output_Name (Get-Files -path $Extraction_Dir -extensions $targets)
    Remove-Item -Path $Extraction_Dir/* -Include $targets
}

function Combine-Libs {
    [string[]] $Extracted_Objs
    foreach($obj_src in $Src_Objs) { $Extracted_Objs += Get-ChildItem -Path $obj_src/* -Include $targets -Recurse }
    Append-To-Lib $Extraction_Dir/$Output_Name $Extracted_Objs
}

Make-Dir $Extraction_Dir

Copy-Item -Path $Src_Libs[0] -Destination $Extraction_Dir/$Output_Name.a

foreach($src in $Src_Libs | select -Skip 1) { Extract-Lib -src_lib $src }

Combine-Libs