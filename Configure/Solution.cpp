/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%  Copyright 2014-2021 ImageMagick Studio LLC, a non-profit organization      %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    http://www.imagemagick.org/script/license.php                            %
%                                                                             %
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#include "stdafx.h"
#include "Solution.h"
#include "Shared.h"
#include "VersionInfo.h"

Solution::Solution(const ConfigureWizard &wizard)
  : _wizard(wizard)
{
}

int Solution::loadProjectFiles() const
{
  int
    count;

  count=0;
  for (auto& project : _projects)
  {
    if (!project->isSupported(_wizard.visualStudioVersion()))
      continue;

    if (!project->loadFiles())
      continue;

    for (auto& projectFile : project->files())
    {
      projectFile->loadConfig();
      count++;
    }

    project->checkFiles(_wizard.visualStudioVersion());

    project->mergeProjectFiles();
  }

  return(count);
}

void Solution::loadProjects()
{
  loadProjectsFromFolder(L"Dependencies", L"Dependencies");
  loadProjectsFromFolder(L"OptionalDependencies", L"OptionalDependencies");
  loadProjectsFromFolder(L"Projects", L"ImageMagick");
}

void Solution::write(WaitDialog &waitDialog) const
{
  int
    steps;

  VersionInfo
    versionInfo;

  wofstream
    file;

  steps=loadProjectFiles();
  waitDialog.setSteps(steps+4);

  file.open(getFileName());
  if (!file)
    return;

  waitDialog.nextStep(L"Writing solution");

  write(file);

  file.close();

  for (auto& project : _projects)
  {
    for (auto& projectFile : project->files())
    {
      waitDialog.nextStep(L"Writing: " + projectFile->fileName());
      projectFile->write(_projects);
    }
  }

  waitDialog.nextStep(L"Writing configuration");
  writeMagickBaseConfig();

  waitDialog.nextStep(L"Writing Makefile.PL");
  writeMakeFile();

  waitDialog.nextStep(L"Writing config config");
  createConfigFiles();

  waitDialog.nextStep(L"Writing threshold-map.h");
  writeThresholdMap();

  if (!versionInfo.load())
    return;

  waitDialog.nextStep(L"Writing version");
  writeVersion(versionInfo);

  waitDialog.nextStep(L"Writing installer config");
  writeInstallerConfig(versionInfo);

  waitDialog.nextStep(L"Writing NOTICE.txt");
  writeNotice(versionInfo);
}

const wstring Solution::getFileName() const
{
  return(pathFromRoot(_wizard.solutionName() + L".sln"));
}

void Solution::loadProjectsFromFolder(const wstring &configFolder, const wstring &filesFolder)
{
  Project
    *project;

  for (const auto& entry : filesystem::directory_iterator(pathFromRoot(configFolder)))
  {
    if (!entry.is_directory())
      continue;

    project=Project::create(_wizard,configFolder,filesFolder,entry.path().filename());
    if (project != (Project *) NULL)
    {
      project->updateProjectNames();
      _projects.push_back(project);
    }
  }
}

void Solution::replaceVersionVariables(const VersionInfo &versionInfo,wifstream &input,wofstream &output) const
{
  size_t
    start,
    end;

  wstring
    line;

  while (getline(input,line))
  {
    line=replace(line,L"@CC@",_wizard.visualStudioVersionName());
    line=replace(line,L"@CHANNEL_MASK_DEPTH@",_wizard.channelMaskDepth());
    line=replace(line,L"@CXX@",_wizard.visualStudioVersionName());
    line=replace(line,L"@DOCUMENTATION_PATH@",L"unavailable");
    line=replace(line,L"@LIB_VERSION@",versionInfo.version());
    line=replace(line,L"@MAGICK_GIT_REVISION@",versionInfo.gitRevision());
    line=replace(line,L"@MAGICK_LIB_VERSION_NUMBER@",versionInfo.libVersionNumber());
    line=replace(line,L"@MAGICK_LIB_VERSION_TEXT@",versionInfo.version());
    line=replace(line,L"@MAGICK_LIBRARY_CURRENT@",versionInfo.interfaceVersion());
    line=replace(line,L"@MAGICK_LIBRARY_CURRENT_MIN@",versionInfo.interfaceVersion());
    line=replace(line,L"@MAGICK_TARGET_CPU@",_wizard.platformAlias());
    line=replace(line,L"@MAGICK_TARGET_OS@",L"Windows");
    line=replace(line,L"@MAGICKPP_LIB_VERSION_TEXT@",versionInfo.version());
    line=replace(line,L"@MAGICKPP_LIBRARY_CURRENT@",versionInfo.ppInterfaceVersion());
    line=replace(line,L"@MAGICKPP_LIBRARY_CURRENT_MIN@",versionInfo.ppInterfaceVersion());
    line=replace(line,L"@MAGICKPP_LIBRARY_VERSION_INFO@",versionInfo.ppLibVersionNumber());
    line=replace(line,L"@MAGICKPP_LIBRARY_VERSION_TEXT@",versionInfo.version());
    line=replace(line,L"@PACKAGE_BASE_VERSION@",versionInfo.version());
    line=replace(line,L"@PACKAGE_FULL_VERSION@",versionInfo.fullVersion());
    line=replace(line,L"@PACKAGE_LIB_VERSION@",versionInfo.libVersion());
    line=replace(line,L"@PACKAGE_LIB_VERSION_NUMBER@",versionInfo.versionNumber());
    line=replace(line,L"@PACKAGE_NAME@",L"ImageMagick");
    line=replace(line,L"@PACKAGE_VERSION_ADDENDUM@",versionInfo.libAddendum());
    line=replace(line,L"@PACKAGE_RELEASE_DATE@",versionInfo.releaseDate());
    line=replace(line,L"@QUANTUM_DEPTH@",_wizard.quantumDepthBits());
    line=replace(line,L"@RELEASE_DATE@",versionInfo.releaseDate());
    line=replace(line,L"@TARGET_OS@",L"Windows");
    start=line.find(L"@");
    if (start != string::npos)
    {
      end=line.find(L"@",start+1);
      if (end != string::npos)
        checkKeyword(line.substr(start+1,end-start-1));
      continue;
    }
    output << line << endl;
  }
}

void Solution::writeInstallerConfig(const VersionInfo &versionInfo) const
{
  wifstream
    inputStream;

  wofstream
    outputStream;

  inputStream.open(pathFromRoot(L"Installer\\Inno\\config.isx.in"));
  if (!inputStream)
    throwException(L"Unable to open installer config input file");

  outputStream.open(pathFromRoot(L"Installer\\Inno\\config.isx"));;
  if (!outputStream)
    {
      inputStream.close();
      throwException(L"Unable to open installer config output file");
    }

  replaceVersionVariables(versionInfo,inputStream,outputStream);

  switch (_wizard.solutionType())
  {
    case SolutionType::DYNAMIC_MT:
      outputStream << L"#define public MagickDynamicPackage 1" << endl;
      if (_wizard.platform() != Platform::ARM64)
        outputStream << L"#define public MagickPerlMagick 1" << endl;
      break;
    case SolutionType::STATIC_MT:
    case SolutionType::STATIC_MTD:
      outputStream << L"#define public MagickStaticPackage 1" << endl;
      break;
  }

  switch (_wizard.platform())
  {
    case Platform::ARM64:
      outputStream << L"#define public MagickArm64Architecture 1" << endl;
      break;
    case Platform::X64:
      outputStream << L"#define public Magick64BitArchitecture 1" << endl;
      break;
  }

  if (_wizard.useHDRI())
      outputStream << L"#define public MagickHDRI 1" << endl;

  if (_wizard.isImageMagick7())
    outputStream << L"#define public MagickVersion7 1" << endl;

  inputStream.close();
  outputStream.close();
}

void Solution::writeMagickBaseConfig() const
{
  wstring
    folder,
    line;

  wifstream
    configIn;

  wofstream
    config;

  wstring
    folderName;

  configIn.open(pathFromRoot(L"Projects\\MagickCore\\magick-baseconfig.h.in"));
  if (!configIn)
    return;

  folderName=_wizard.magickCoreProjectName();
  config.open(pathFromRoot(L"ImageMagick\\" + folderName + L"\\magick-baseconfig.h"));
  if (!config)
    return;

  while (getline(configIn,line))
  {
    if (trim(line).compare(L"$$CONFIG$$") != 0)
    {
      config << line << endl;
      continue;
    }

    config << "/*" << endl;
    config << "  Define to build a ImageMagick which uses registry settings or" << endl;
    config << "  hard-coded paths to locate installed components.  This supports" << endl;
    config << "  using the \"setup.exe\" style installer, or using hard-coded path" << endl;
    config << "  definitions (see below).  If you want to be able to simply copy" << endl;
    config << "  the built ImageMagick to any directory on any directory on any machine," << endl;
    config << "  then do not use this setting." << endl;
    config << "*/" << endl;
    if (_wizard.installedSupport())
      config << "#define MAGICKCORE_INSTALLED_SUPPORT" << endl;
    else
      config << "#undef MAGICKCORE_INSTALLED_SUPPORT" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Specify size of PixelPacket color Quantums (8, 16, or 32)." << endl;
    config << "  A value of 8 uses half the memory than 16 and typically runs 30% faster," << endl;
    config << "  but provides 256 times less color resolution than a value of 16." << endl;
    config << "*/" << endl;
    if (_wizard.quantumDepth() == QuantumDepth::Q8)
      config << "#define MAGICKCORE_QUANTUM_DEPTH 8" << endl;
    else if (_wizard.quantumDepth() == QuantumDepth::Q16)
      config << "#define MAGICKCORE_QUANTUM_DEPTH 16" << endl;
    else if (_wizard.quantumDepth() == QuantumDepth::Q32)
      config << "#define MAGICKCORE_QUANTUM_DEPTH 32" << endl;
    else if (_wizard.quantumDepth() == QuantumDepth::Q64)
      config << "#define MAGICKCORE_QUANTUM_DEPTH 64" << endl;
    config << endl;

    if (_wizard.channelMaskDepth() != L"")
      {
        config << "/*" << endl;
        config << "  Channel mask depth" << endl;
        config << "*/" << endl;
        config << "#define MAGICKCORE_CHANNEL_MASK_DEPTH " << _wizard.channelMaskDepth() << endl;
        config << endl;
      }

    config << "/*" << endl;
    config << "  Define to enable high dynamic range imagery (HDRI)" << endl;
    config << "*/" << endl;
    if (_wizard.useHDRI())
      config << "#define MAGICKCORE_HDRI_ENABLE 1" << endl;
    else
      config << "#define MAGICKCORE_HDRI_ENABLE 0" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Define to enable OpenCL" << endl;
    config << "*/" << endl;
    if (_wizard.useOpenCL())
      config << "#define MAGICKCORE_HAVE_CL_CL_H" << endl;
    else
      config << "#undef MAGICKCORE_HAVE_CL_CL_H" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Define to enable Distributed Pixel Cache" << endl;
    config << "*/" << endl;
    if (_wizard.enableDpc())
      config << "#define MAGICKCORE_DPC_SUPPORT" << endl;
    else
      config << "#undef MAGICKCORE_DPC_SUPPORT" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Exclude deprecated methods in MagickCore API" << endl;
    config << "*/" << endl;
    if (_wizard.excludeDeprecated())
      config << "#define MAGICKCORE_EXCLUDE_DEPRECATED" << endl;
    else
      config << "#undef MAGICKCORE_EXCLUDE_DEPRECATED" << endl;
    config << endl;

    config << "/*" << endl;
    config << "  Define to only use the built-in (in-memory) settings." << endl;
    config << "*/" << endl;
    if (_wizard.zeroConfigurationSupport())
      config << "#define MAGICKCORE_ZERO_CONFIGURATION_SUPPORT 1" << endl;
    else
      config << "#define MAGICKCORE_ZERO_CONFIGURATION_SUPPORT 0" << endl;

    for (const auto& project : _projects)
    {
      if (project->files().size() == 0)
        continue;

      if (project->configDefine().empty())
        continue;

      config << endl;
      config << project->configDefine();
    }
  }
}

void Solution::writeMakeFile() const
{
  wifstream
    makeFileIn,
    zipIn;

  wofstream
    lib,
    makeFile,
    zip;

  wstring
    libName,
    line;

  libName=L"CORE_RL_" + _wizard.magickCoreProjectName()+ L"_";

  lib=wofstream(pathFromRoot(L"ImageMagick\\PerlMagick\\" + libName + L".a"));
  if (!lib)
    return;

  lib.close();

  zipIn=wifstream(pathFromRoot(L"Projects\\PerlMagick\\Zip.ps1"), std::ios::binary);
  if (!zipIn)
    return;

  zip=wofstream(pathFromRoot(L"ImageMagick\\PerlMagick\\Zip.ps1"), std::ios::binary);
  zip << zipIn.rdbuf();
  zip.close();

  makeFileIn.open(pathFromRoot(L"Projects\\PerlMagick\\Makefile.PL.in"));
  if (!makeFileIn)
    return;

  makeFile.open(pathFromRoot(L"ImageMagick\\PerlMagick\\Makefile.PL"));
  if (!makeFile)
    return;

  while (getline(makeFileIn,line))
  {
    line=replace(line,L"$$LIB_NAME$$",libName);
    line=replace(line,L"$$PLATFORM$$",_wizard.platformAlias());
    makeFile << line << endl;
  }
  makeFile.close();
}

void Solution::writeNotice(const VersionInfo &versionInfo) const
{
  wofstream
    notice;

  notice.open(pathFromRoot(L"Output\\NOTICE.txt"));
  if (!notice)
    return;

  notice << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl << endl;
  notice << "[ Imagemagick " << versionInfo.version() << versionInfo.libAddendum() << "] copyright:" << endl << endl;
  notice << readLicense(pathFromRoot(L"ImageMagick\\LICENSE"));
  notice << endl << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl << endl;

  for (const auto& project : _projects)
  {
    if (project->notice() == L"" || project->shouldSkip())
      continue;

    notice << project->notice();
    notice << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl << endl;
  }

  notice.close();
}

void Solution::writeThresholdMap() const
{
  wifstream
    inputStream;

  wofstream
    outputStream;

  wstring
    fileName,
    line;

  if (!_wizard.zeroConfigurationSupport())
    return;

  fileName=pathFromRoot(_wizard.binDirectory() + L"thresholds.xml");
  inputStream.open(fileName);
  if (!inputStream)
    throwException(L"Unable to open:" + fileName);

  fileName=pathFromRoot(L"ImageMagick\\" + _wizard.magickCoreProjectName() + L"\\threshold-map.h");
  outputStream.open(fileName);
  if (!outputStream)
    {
      inputStream.close();
      throwException(L"Unable to open:" + fileName);
    }

  outputStream << "static const char *const BuiltinMap=" << endl;

  while (getline(inputStream,line))
  {
    if (line.length() == 0)
      continue;

    line=replace(line,L"\"",L"\\\"");
    outputStream << "\"" << line << "\"" << endl;
  }

  outputStream << ";";

  inputStream.close();
  outputStream.close();
}

void Solution::writeVersion(const VersionInfo &versionInfo) const
{
  wstring
    folderName,
    line;

  folderName=_wizard.magickCoreProjectName();
  writeVersion(versionInfo,pathFromRoot(L"ImageMagick\\" + folderName + L"\\version.h.in"),pathFromRoot(L"ImageMagick\\" + folderName + L"\\version.h"));
  filesystem::copy_file(pathFromRoot(L"ImageMagick\\" + folderName + L"\\version.h"),pathFromRoot(L"Build\\version.h"),filesystem::copy_options::overwrite_existing);
  writeVersion(versionInfo,pathFromRoot(L"ImageMagick\\config\\configure.xml.in"),pathFromRoot(_wizard.binDirectory() + L"configure.xml"));
  writeVersion(versionInfo,pathFromRoot(L"Projects\\utilities\\ImageMagick.version.h.in"),pathFromRoot(L"Projects\\utilities\\ImageMagick.version.h"));
}

void Solution::writeVersion(const VersionInfo &versionInfo,const wstring &input,const wstring &output) const
{
  wifstream
    inputStream;

  wofstream
    outputStream;

  inputStream.open(input);
  if (!inputStream)
    throwException(L"Unable to open: " + input);

  outputStream.open(output);
  if (!outputStream)
    {
      inputStream.close();
      throwException(L"Unable to open: " + output);
    }

  replaceVersionVariables(versionInfo,inputStream,outputStream);

  inputStream.close();
  outputStream.close();
}

void Solution::checkKeyword(const wstring keyword) const
{
  vector<wstring> skipableKeywords={
    L"CODER_PATH",L"CONFIGURE_ARGS",L"CONFIGURE_PATH",L"CXXFLAGS",L"DEFS",L"DISTCHECK_CONFIG_FLAGS",
    L"EXEC_PREFIX_DIR",L"EXECUTABLE_PATH",L"FILTER_PATH",L"host",L"INCLUDE_PATH",L"LIBRARY_PATH",
    L"MAGICK_CFLAGS",L"MAGICK_CPPFLAGS",L"MAGICK_DELEGATES",L"MAGICK_FEATURES",L"MAGICK_LDFLAGS",
    L"MAGICK_LIBS",L"MAGICK_PCFLAGS",L"MAGICK_SECURITY_POLICY",L"MAGICK_TARGET_VENDOR",L"PREFIX_DIR",
    L"SHARE_PATH",L"SHAREARCH_PATH"
  };

  if (contains(skipableKeywords,keyword))
    return;

  throwException(L"Invalid keyword: " + keyword);
}

void Solution::createConfigFiles() const
{
  wstring
    policyXml;

  vector<wstring>
    xmlFiles = { L"colors.xml", L"english.xml", L"locale.xml", L"log.xml", L"thresholds.xml" };

  switch(_wizard.policyConfig())
  {
    case PolicyConfig::LIMITED:
      policyXml=pathFromRoot(L"ImageMagick\\config\\policy-limited.xml");
      break;
    case PolicyConfig::OPEN:
      policyXml=pathFromRoot(L"ImageMagick\\config\\policy-open.xml");
      break;
    case PolicyConfig::SECURE:
      policyXml=pathFromRoot(L"ImageMagick\\config\\policy-secure.xml");
      break;
    case PolicyConfig::WEBSAFE:
      policyXml=pathFromRoot(L"ImageMagick\\config\\policy-websafe.xml");
      break;
  }
  if (!filesystem::exists(policyXml))
    throwException(L"Unable to open policy file");
  filesystem::copy_file(policyXml,pathFromRoot(_wizard.binDirectory() + L"policy.xml"),filesystem::copy_options::overwrite_existing);
  for (auto& xmlFile : xmlFiles)
  {
    filesystem::copy_file(pathFromRoot(L"ImageMagick\\config\\" + xmlFile),pathFromRoot(_wizard.binDirectory() + xmlFile),filesystem::copy_options::overwrite_existing);
  }  
}

void Solution::write(wofstream &file) const
{
  file << "Microsoft Visual Studio Solution File, Format Version 12.00" << endl;
  if (_wizard.visualStudioVersion() == VisualStudioVersion::VS2017)
    file << "# Visual Studio 2017" << endl;
  else if (_wizard.visualStudioVersion() == VisualStudioVersion::VS2019)
    file << "# Visual Studio 2019" << endl;
  else if (_wizard.visualStudioVersion() == VisualStudioVersion::VS2022)
    file << "# Visual Studio 2022" << endl;

  for (auto& project : _projects)
  {
    for (auto& projectFile : project->files())
    {
      file << "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"" << projectFile->name() << "\", ";
      file << "\"" << _wizard.solutionName() << ".Projects\\" << projectFile->name() << "\\" << projectFile->fileName() << "\", \"{" << projectFile->guid() << "}\"" << endl;
      file << "EndProject" << endl;
    }
  }
  file << "EndProject" << endl;

  file << "Global" << endl;
  file << "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution" << endl;
  file << "\t\tDebug|" << _wizard.platformAlias() << " = Debug|" << _wizard.platformAlias() << endl;
  file << "\t\tRelease|" << _wizard.platformAlias() << " = Release|" << _wizard.platformAlias() << endl;
  file << "\tEndGlobalSection" << endl;

  file << "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution" << endl;
  for (auto& project : _projects)
  {
    for (auto& projectFile : project->files())
    {
      file << "\t\t{" << projectFile->guid() << "}.Debug|" << _wizard.platformAlias() << ".ActiveCfg = Debug|" << _wizard.platformName() << endl;
      file << "\t\t{" << projectFile->guid() << "}.Debug|" << _wizard.platformAlias() << ".Build.0 = Debug|" << _wizard.platformName() << endl;
      file << "\t\t{" << projectFile->guid() << "}.Release|" << _wizard.platformAlias() << ".ActiveCfg = Release|" << _wizard.platformName() << endl;
      file << "\t\t{" << projectFile->guid() << "}.Release|" << _wizard.platformAlias() << ".Build.0 = Release|" << _wizard.platformName() << endl;
    }
  }
  file << "\tEndGlobalSection" << endl;
  file << "EndGlobal" << endl;
}
