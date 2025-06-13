###############################################################################
# Copyright ImageMagick 2015-2016
###############################################################################

Add-Type -AssemblyName System.IO.Compression.FileSystem

$fileName = $args[0]
$directory = $args[1]

if (Test-path $fileName)
{
  Remove-item $fileName
}

$compressionLevel = [System.IO.Compression.CompressionLevel]::Optimal
[System.IO.Compression.ZipFile]::CreateFromDirectory($directory, $fileName, $compressionLevel, $false)