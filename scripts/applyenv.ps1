$files = Get-Content -Path $Args[0]

$files | ForEach-Object {

    $variable=$_ -split "="

    $env_name = $variable[0]
    $env_value = $variable[1]

    Set-Item -Path env:$env_name -Value "$env_value"
}