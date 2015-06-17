function GetConfig($platform, $name)
{
  $config = $null

  if ($name -eq "dll-Q8")
  {
    $config = @{options="/dmt /noHdri /Q8 /includeOptional /installedSupport";perl=$true;type="installer";solution="VisualDynamicMT.sln"}
  }
  elseif ($name -eq "dll-Q16")
  {
    $config = @{options="/dmt /noHdri /Q16 /includeOptional /installedSupport";perl=$true;type="installer";solution="VisualDynamicMT.sln"}
  }
  elseif ($name -eq "static-Q8")
  {
    $config = @{options="/smtd /noHdri /Q8 /includeOptional /installedSupport";perl=$false;type="installer";solution="VisualStaticMTD.sln"}
  }
  elseif ($name -eq "static-Q16")
  {
    $config = @{options="/smtd /noHdri /Q16 /includeOptional /installedSupport";perl=$false;type="installer";solution="VisualStaticMTD.sln"}
  }

  elseif ($name -eq "hdri-dll-Q16")
  {
    $config = @{options="/dmt /hdri /Q16 /includeOptional /installedSupport";perl=$true;type="installer";solution="VisualDynamicMT.sln"}
  }
  elseif ($name -eq "hdri-static-Q16")
  {
    $config = @{options="/smtd /hdri /Q16 /includeOptional /installedSupport";perl=$false;type="installer";solution="VisualStaticMTD.sln"}
  }

  elseif ($name -eq "portable-Q16")
  {
    $config = @{options="/smtd /Q16";perl=$false;type="portable";solution="VisualStaticMTD.sln"}
  }

  if ($config -ne $null)
  {
    $config.platform = $platform
    $config.name = $name
  }

  return $config
}

function GetVersion()
{
  return Get-Content "Version.txt"
}

function CheckExitCode($msg)
{
	if ($LastExitCode -ne 0)
	{
		Throw $msg
	}
}

function BuildConfigure()
{
  Set-Location ../VisualMagick/configure
  devenv /upgrade configure.vcxproj
  msbuild configure.sln /m:4 /t:Rebuild ("/p:Configuration=Release,Platform=Win32")
  CheckExitCode "Failed to build: configure.exe"

  Set-Location ../../AppVeyor
}

function BuildConfiguration($config)
{
  $platformName = "Win32"
  $options = $config.options
  if ($config.platform -eq "x64")
  {
    $platformName = "x64"
    $options = "$options /x64"
  }

  Set-Location ../VisualMagick/configure
  Start-Process configure.exe -ArgumentList "/noWizard /VS2013 $options" -wait

  Set-Location ..
  msbuild $config.solution /m:4 /t:Rebuild ("/p:Configuration=Release,Platform=$platformName")
  CheckExitCode "Failed to build: $($config.name)"

  Set-Location ../AppVeyor

  if ($config.perl -eq $true)
  {
    BuildPerlMagick $platform
  }
}

function BuildPerlMagick($platform)
{
  $folder = "C:\Strawberry-$platform"
  $env:Path = "$($env:Path);$folder\c\bin"

  Set-Location ../ImageMagick/PerlMagick

  & "$folder\perl\bin\perl.exe" "Makefile.PL" "MAKE=nmake"
  CheckExitCode "Failed to create Makefile: $($name)"

  & "nmake"
  CheckExitCode "Failed to build PerlMagick"

  & "nmake" "release"
  CheckExitCode "Failed to build PerlMagick release"

  Set-Location ../../AppVeyor
}

function CreatePackage($config, $version)
{
  if ($config.type -eq "installer")
  {
    CreateInstaller $config
  }
  elseif ($config.type -eq "portable")
  {
    CreatePortable $config $version
  }
  elseif ($config.type -eq "source")
  {
    CreateSource
  }
}

function CreateInstaller($config)
{
  & "C:\Program Files (x86)\Inno Setup 5\iscc.exe" "/cc" "..\VisualMagick\installer\im-$($config.platform)-$($config.name).iss"
  CheckExitCode "Failed to create setup executable."

  Get-ChildItem -Path ..\VisualMagick\installer\output\*.exe -Recurse | Move-Item -Destination ..\Windows-Distribution
}

function CreateZipFile($fileName, $directory)
{
  Write-Host "Creating: $($fileName)"
  Add-Type -Assembly System.IO.Compression.FileSystem
  $compressionLevel = [System.IO.Compression.CompressionLevel]::Optimal
  [System.IO.Compression.ZipFile]::CreateFromDirectory($directory, $fileName, $compressionLevel, $false)
}

function CreatePortable($config, $version)
{
  New-Item -ItemType directory -Path .\Portable | Out-Null

  Copy-Item ..\VisualMagick\bin\*.exe .\Portable
  Copy-Item ..\VisualMagick\bin\*.xml .\Portable
  Copy-Item ..\VisualMagick\bin\ImageMagick.rdf .\Portable

  Copy-Item ..\ImageMagick\ChangeLog .\Portable
  Copy-Item ..\ImageMagick\images .\Portable -recurse
  Copy-Item ..\ImageMagick\index.html .\Portable
  Copy-Item ..\ImageMagick\LICENSE .\Portable
  Copy-Item ..\ImageMagick\NOTICE .\Portable
  Copy-Item ..\ImageMagick\README.txt .\Portable
  Copy-Item ..\ImageMagick\www .\Portable -recurse

  $redist = "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\redist\$($config.platform)\Microsoft.VC120."
  Copy-Item "$($redist)CRT\msvcr120.dll" .\Portable
  Copy-Item "$($redist)CRT\msvcp120.dll" .\Portable
  Copy-Item "$($redist)OPENMP\vcomp120.dll" .\Portable
  Copy-Item "$($redist)MFC\mfc120u.dll" .\Portable

  CheckExitCode "Failed to copy files."

  $output = "..\Windows-Distribution\ImageMagick-$($version)-$($config.name)-$($config.platform).zip"
  CreateZipFile $output ".\Portable"
}

function CreateSource($version)
{
  $folder = ".\Source\ImageMagick-$version"
  New-Item -ItemType directory -Path $folder | Out-Null

  $root = Get-Item ..
  ForEach ($dir in $root.GetDirectories())
  {
    if ($dir.Name -eq "AppVeyor" -Or $dir.Name -eq "Windows-Distribution")
    {
      continue;
    }

    if ($dir.Name -ne "dcraw")
    {
      Remove-Item "$($dir.FullName)\.git" -recurse -force
    }
    Write-Host "Adding $($dir.Name)"
    Copy-Item $dir.FullName $folder -recurse
  }

  $output = "..\Windows-Distribution\ImageMagick-Windows-$($version).zip"
  CreateZipFile $output ".\Source"

  Copy-Item "Version.txt" "..\Windows-Distribution"
}

function CheckUpload($version, $password)
{
  $webclient = New-Object System.Net.WebClient
  $webclient.Credentials = New-Object System.Net.NetworkCredential("upload", $password)

  $source = "ftp://magick.imagemagick.org/ImageMagick-7/Windows-Distribution/Version.txt"
  $destination = "CurrentVersion.txt"
  $webclient.DownloadFile($source, $destination)

  $currentVersion = Get-Content "CurrentVersion.txt"
  if ($currentVersion -eq $version)
  {
    Write-Host "Version $($version) has already been published."
    Remove-Item ..\Windows-Distribution\*
  }
}


$platform = $args[0]
$name = $args[1]
$password = $args[2]

$version = GetVersion

New-Item -ItemType directory -Path ..\Windows-Distribution | Out-Null

if ($name -eq "source")
{
  CreateSource $version
  CheckUpload $version $password
}
else
{
  $config = GetConfig $platform $name
  if ($config -eq $null)
  {
    throw "Unknown configuration: $name"
  }

  BuildConfigure
  BuildConfiguration $config
  CreatePackage $config $version
  CheckUpload $version $password
}
