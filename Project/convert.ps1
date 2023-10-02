$files = Get-Item *.jpg

foreach($f in $files)
{
    Write-Output $f
    Start-Process -FilePath TextureConverter.exe -ArgumentList $f -Wait
}
pause