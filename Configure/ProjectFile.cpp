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
#include "Project.h"
#include "ProjectFile.h"
#include "Shared.h"
#include <algorithm>
#include <map>

static const wstring
  rootPath(L"..\\..\\");

ProjectFile::ProjectFile(const ConfigureWizard *wizard,Project *project,
  const wstring &prefix,const wstring &name)
  : _wizard(wizard),
    _project(project),
    _prefix(prefix),
    _name(name)
{
  initialize(project);

  if (!wizard->excludeAliases())
    loadAliases();
}

ProjectFile::ProjectFile(const ConfigureWizard *wizard,Project *project,
  const wstring &prefix,const wstring &name,const wstring &reference)
  : _wizard(wizard),
    _project(project),
    _prefix(prefix),
    _name(name),
    _reference(reference)
{
  initialize(project);
}

const wstring ProjectFile::binDirectory() const
{
    return(rootPath + _wizard->binDirectory());
}

const vector<wstring> &ProjectFile::dependencies() const
{
  return(_dependencies);
}

const wstring ProjectFile::fileName() const
{
  return(_fileName);
}

const wstring ProjectFile::guid() const
{
  return(_guid);
}

const wstring ProjectFile::name() const
{
  return(_prefix+L"_"+_name);
}

const vector<wstring> &ProjectFile::aliases() const
{
  return(_aliases);
}

void ProjectFile::initialize(Project* project)
{
  _minimumVisualStudioVersion=VSEARLIEST;
  setFileName();
  _guid=createGuid();

  for (auto& dep : project->dependencies())
  {
    _dependencies.push_back(dep);
  }

  for (auto& inc : project->includes())
  {
    _includes.push_back(inc);
  }

  for (auto& inc : project->definesLib())
  {
    _definesLib.push_back(inc);
  }
}

bool ProjectFile::isSrcFile(const wstring &fileName)
{
  if (endsWith(fileName,L".asm"))
    return(true);

  return(isValidSrcFile(fileName));
}

bool ProjectFile::isExcluded(const wstring &fileName)
{
  if (contains(_project->excludes(),fileName))
    return true;

  if (contains(_project->platformExcludes(_wizard->platform()),fileName))
    return true;

  if (endsWith(fileName,L".h"))
    return isExcluded(replace(fileName,L".h",L".c")) || isExcluded(replace(fileName,L".h",L".cc"));

  return false;
}

void ProjectFile::loadAliases()
{
  wifstream
    aliases;

  wstring
    fileName,
    line;

  if (!_project->isExe() || !_project->isModule())
    return;

  fileName=pathFromRoot(_project->configPath(L"Aliases." + _name + L".txt"));

  aliases.open(fileName);
  if (!aliases)
    return;

  while (!aliases.eof())
  {
    line=readLine(aliases);
    if (!line.empty())
      _aliases.push_back(line);
  }

  aliases.close();
}

bool ProjectFile::isSupported(const VisualStudioVersion visualStudioVersion) const
{
  return(visualStudioVersion >= _minimumVisualStudioVersion);
}

void ProjectFile::loadConfig()
{
  wifstream
    config;

  wstring
    fileName,
    line;

  if (!_project->isModule())
    return;

  fileName=pathFromRoot(_project->configPath(L"Config." + _name + L".txt"));

  config.open(fileName);
  if (!config)
    return;

  while (!config.eof())
  {
    line=readLine(config);
    if (line == L"[DEPENDENCIES]")
      addLines(config,_dependencies);
    else if (line == L"[INCLUDES]")
      addLines(config,_includes);
    else if (line == L"[CPP]")
      addLines(config,_cppFiles);
    else if (line == L"[VISUAL_STUDIO]")
      _minimumVisualStudioVersion=parseVisualStudioVersion(readLine(config));
    else if (line == L"[DEFINES_LIB]")
      addLines(config,_definesLib);
  }

  config.close();
}

void ProjectFile::merge(ProjectFile *projectFile)
{
  merge(projectFile->_dependencies,_dependencies);
  merge(projectFile->_includes,_includes);
  merge(projectFile->_cppFiles,_cppFiles);
  merge(projectFile->_definesLib,_definesLib);
}

void ProjectFile::write(const vector<Project*> &allprojects)
{
  wofstream
    file;

  wstring
    projectDir(pathFromRoot(_wizard->solutionName() + L".Projects\\" + name()));

  filesystem::create_directories(projectDir.c_str());

  file.open(projectDir + L"\\" + _fileName);
  if (!file)
    return;

  loadSource();

  write(file,allprojects);

  file.close();

  file.open(projectDir + L"\\" + _fileName + L".filters");
  if (!file)
    return;

  writeFilter(file);

  file.close();
}

bool ProjectFile::isLib() const
{
  return(_project->isLib() || (_wizard->solutionType() != SolutionType::DYNAMIC_MT && _project->isDll()));
}

const wstring ProjectFile::libDirectory() const
{
    return(rootPath + L"Output\\lib\\");
}

const wstring ProjectFile::outputDirectory() const
{
  if (_project->isFuzz())
    return(rootPath + L"Output\\fuzz\\");

  if (isLib())
    return(libDirectory());

  return(binDirectory());
}

void ProjectFile::addFile(const wstring &name)
{
  wstring
    header_file,
    src_file;

  for (auto& ext : validSrcFiles)
  {
    src_file=_project->filePath(name + ext);

    if (filesystem::exists(pathFromRoot(src_file)))
    {
      _srcFiles.push_back(rootPath + src_file);

      header_file=_project->filePath(name + L".h");
      if (filesystem::exists(pathFromRoot(header_file)))
        _includeFiles.push_back(rootPath + header_file);

      break;
    }
  }

  if (!_project->isExe())
    return;

  for (auto& ext : validSrcFiles)
  {
    src_file=_project->filePath(L"main" + ext);

    if (filesystem::exists(pathFromRoot(src_file)))
    {
      _srcFiles.push_back(rootPath + src_file);

      header_file=_project->filePath(name + L".h");
      if (filesystem::exists(pathFromRoot(header_file)))
        _includeFiles.push_back(rootPath + header_file);

      break;
    }
  }
}

void ProjectFile::addLines(wifstream &config,vector<wstring> &container)
{
  wstring
    line;

  while (!config.eof())
  {
    line=readLine(config);
    if (line.empty())
      return;

    if (!contains(container,line))
      container.push_back(line);
  }
}

const wstring ProjectFile::asmOptions() const
{
  switch (_wizard->platform())
  {
    case Platform::X86: return(L"ml /nologo /c /Cx /safeseh /coff /Fo\"$(IntDir)%(Filename).obj\" \"%(FullPath)\"");
    case Platform::X64: return(L"ml64 /nologo /c /Cx /Fo\"$(IntDir)%(Filename).obj\" \"%(FullPath)\"");
    case Platform::ARM64: return(L"armasm64 \"%(FullPath)\" -o \"$(IntDir)%(Filename).obj\"");
    default: throw;
  }
}

const wstring ProjectFile::getFilter(const wstring &fileName,vector<wstring> &filters) const
{
  wstring
    filter,
    folder;

  filter=replace(fileName,L"..\\",L"");
  if (filter.find_first_of(L"\\") == filter.find_last_of(L"\\"))
    return L"";

  filter=filter.substr(filter.find_first_of(L"\\") + 1);

  folder=filter;
  while (folder.find_first_of(L"\\") != -1)
  {
    folder=folder.substr(0, folder.find_last_of(L"\\"));
    if (!contains(filters,folder))
      filters.push_back(folder);
  }

  filter=filter.substr(0, filter.find_last_of(L"\\"));

  return filter;
}

const wstring ProjectFile::getIntermediateDirectoryName(const bool debug) const
{
  return((debug ? L"Debug\\" : L"Release\\") + _wizard->platformName() + L"\\");
}

const wstring ProjectFile::getTargetName(const bool debug) const
{
  wstring
    targetName;

  targetName=_prefix+L"_";
  if (_prefix.compare(L"FILTER") != 0)
    targetName+=wstring(debug ? L"DB" : L"RL")+L"_";
  targetName+=_name+L"_";
  return(targetName);
}

void ProjectFile::loadModule()
{
  if (!_reference.empty())
    addFile(_reference);
  else
    addFile(_name);
}

void ProjectFile::loadSource()
{
  wstring
    resourceFile;

  for (auto& dir : _project->directories())
  {
    if ((_project->isModule()) && (_project->isExe() || (_project->isDll() && _wizard->solutionType() == SolutionType::DYNAMIC_MT)))
      loadModule();
    else
      loadSource(dir);
  }

  resourceFile=_project->configPath(L"ImageMagick.rc");
  if (filesystem::exists(pathFromRoot(resourceFile)))
    _resourceFiles.push_back(rootPath + resourceFile);
}

void ProjectFile::loadSource(const wstring &directory)
{
  wstring
    path=_project->filePath(directory);

  if (contains(_project->platformExcludes(_wizard->platform()),directory))
    return;

  if (!directoryExists(pathFromRoot(path)))
    throwException(L"Invalid folder specified: " + path);

  for (const auto& entry : filesystem::directory_iterator(pathFromRoot(path)))
  {
    wstring
      fileName;

    if (!entry.is_regular_file())
      continue;

    fileName=entry.path().filename();
    if (isExcluded(fileName))
      continue;

    if (isSrcFile(fileName))
      _srcFiles.push_back(rootPath + path + L"\\" +fileName);
    else if (endsWith(fileName,L".h"))
      _includeFiles.push_back(rootPath + path + L"\\" + fileName);
    else if (endsWith(fileName,L".rc"))
      _resourceFiles.push_back(rootPath + path + L"\\" + fileName);
  }
}

const wstring ProjectFile::nasmOptions(const wstring &folder) const
{
  wstring
    result=L"";

  if (_wizard->platform() == Platform::ARM64)
    return(result);

  result += rootPath + L"Build\\nasm -i\"" + folder +L"\"";

  if (_wizard->platform() == Platform::X86)
    result += L" -fwin32 -DWIN32";
  else
    result += L" -fwin64 -DWIN64 -D__x86_64__";

  for (auto& include : _project->includesNasm())
    result += L" -i\"" + rootPath + L"\\" + _project->filePath(include) + L"\"";

  result += L" -o \"$(IntDir)%(Filename).obj\" \"%(FullPath)\"";
  return(result);
}

void ProjectFile::merge(vector<wstring> &input, vector<wstring> &output)
{
  for (auto& value : input)
  {
    if (!contains(output,value))
      output.push_back(value);
  }
}

void ProjectFile::setFileName()
{
  _fileName=_prefix+L"_"+_name+L".vcxproj";
}

const wstring ProjectFile::createGuid() const
{
  hash<string>
    hash;

  size_t
    hash_value;

  wstringstream
    wss;

  wstring
    guid;

  hash_value=hash(wstringToString(name()));
  wss << hex << setw(16) << setfill(L'0') << hash_value;
  wss << hex << setw(16) << setfill(L'0') << hash_value;
  guid=wss.str();
  guid.insert(20, 1, '-');
  guid.insert(16, 1, '-');
  guid.insert(12, 1, '-');
  guid.insert(8, 1, '-');
  return(guid);
}

void ProjectFile::write(wofstream &file,const vector<Project*> &allProjects) const
{
  file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
  file << "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">" << endl;
  file << "  <ItemGroup Label=\"ProjectConfigurations\">" << endl;
  file << "    <ProjectConfiguration Include=\"Debug|" << _wizard->platformName() << "\">" << endl;
  file << "      <Configuration>Debug</Configuration>" << endl;
  file << "      <Platform>" << _wizard->platformName() << "</Platform>" << endl;
  file << "    </ProjectConfiguration>" << endl;
  file << "    <ProjectConfiguration Include=\"Release|" << _wizard->platformName() << "\">" << endl;
  file << "      <Configuration>Release</Configuration>" << endl;
  file << "      <Platform>" << _wizard->platformName() << "</Platform>" << endl;
  file << "    </ProjectConfiguration>" << endl;
  file << "  </ItemGroup>" << endl;
  file << "  <PropertyGroup Label=\"Globals\">" << endl;
  file << "    <ProjectName>" << _prefix << "_" << _name << "</ProjectName>" << endl;
  file << "    <ProjectGuid>{" << _guid << "}</ProjectGuid>" << endl;
  file << "    <Keyword>" << _wizard->platformName() << "Proj</Keyword>" << endl;
  file << "  </PropertyGroup>" << endl;
  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />" << endl;

  file << "  <PropertyGroup Label=\"Configuration\">" << endl;
  if (isLib())
    file << "    <ConfigurationType>StaticLibrary</ConfigurationType>" << endl;
  else if (_project->isDll())
    file << "    <ConfigurationType>DynamicLibrary</ConfigurationType>" << endl;
  else if (_project->isExe())
    file << "    <ConfigurationType>Application</ConfigurationType>" << endl;
  if (_wizard->visualStudioVersion() == VisualStudioVersion::VS2017)
    file << "    <PlatformToolset>v141</PlatformToolset>" << endl;
  else if (_wizard->visualStudioVersion() == VisualStudioVersion::VS2019)
    file << "    <PlatformToolset>v142</PlatformToolset>" << endl;
  else if (_wizard->visualStudioVersion() == VisualStudioVersion::VS2022)
    file << "    <PlatformToolset>v143</PlatformToolset>" << endl;
  file << "    <UseOfMfc>false</UseOfMfc>" << endl;
  if (_project->useUnicode())
    file << "    <CharacterSet>Unicode</CharacterSet>" << endl;
  else
    file << "    <CharacterSet>MultiByte</CharacterSet>" << endl;
  file << "  </PropertyGroup>" << endl;

  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />" << endl;

  file << "  <PropertyGroup>" << endl;
  file << "    <LinkIncremental>false</LinkIncremental>" << endl;
  file << "    <OutDir>" << outputDirectory() << "</OutDir>" << endl;
  if (_project->isExe())
  {
    file << "    <TargetName>" << _name << "</TargetName>" << endl;
  }
  else
  {
    file << "    <TargetName Condition=\"'$(Configuration)|$(Platform)'=='Debug|" << _wizard->platformName() << "'\">" << getTargetName(true) << "</TargetName>" << endl;
    file << "    <TargetName Condition=\"'$(Configuration)|$(Platform)'=='Release|" << _wizard->platformName() << "'\">" << getTargetName(false) << "</TargetName>" << endl;
  }
  file << "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Debug|" << _wizard->platformName() << "'\">" << getIntermediateDirectoryName(true) << "</IntDir>" << endl;
  file << "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Release|" << _wizard->platformName() << "'\">" << getIntermediateDirectoryName(false) << "</IntDir>" << endl;
  if (_wizard->visualStudioVersion() >= VisualStudioVersion::VS2019)
    file << "    <UseDebugLibraries Condition=\"'$(Configuration)|$(Platform)'=='Debug|" << _wizard->platformName() << "'\">true</UseDebugLibraries>" << endl;
  file << "  </PropertyGroup>" << endl;

  writeItemDefinitionGroup(file,true,allProjects);
  writeItemDefinitionGroup(file,false,allProjects);

  writeFiles(file,_srcFiles);
  writeFiles(file,_includeFiles);
  writeFiles(file,_resourceFiles);

  writeProjectReferences(file,allProjects);

  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />" << endl;
  file << "</Project>" << endl;
}

void ProjectFile::writeAdditionalDependencies(wofstream &file,const wstring &separator) const
{
  for (auto& lib : _project->libraries())
    file << separator << lib;
}

void ProjectFile::writeAdditionalIncludeDirectories(wofstream &file,const wstring &separator,const vector<Project*> &allProjects) const
{
  size_t
    index;

  for (auto& includeDir :_includes)
  {
    const Project
      *project;

    wstring
      directory;

    project=_project;
    directory=includeDir;
    index=includeDir.find(L"->");
    if (index != -1)
    {
      wstring
        projectName;

      project=(const Project *) NULL;
      projectName=directory.substr(0,index);
      for (const auto& depp : allProjects)
      {
        if (depp->name() != projectName)
          continue;

        directory=directory.substr(index+2);
        project=depp;
        break;
      }

      if (project == (const Project *) NULL)
        throwException(L"Invalid dependency specified: " + projectName);
    }
    
    file << separator << rootPath << project->filePath(directory);
  }

  if (_wizard->useOpenCL() && _project->useOpenCL())
    file << separator << rootPath << L"Build\\OpenCL";
}

void ProjectFile::writeFiles(wofstream &file,const vector<wstring> &collection) const
{
  int
    count;

  map<wstring, int>
    fileCount;

  wstring
    fileName,
    folder,
    name;

  if (collection.size() == 0)
    return;

  file << "  <ItemGroup>" << endl;
  for (auto& f : collection)
  {
    if (endsWith(f,L".rc"))
      file << "    <ResourceCompile Include=\"" << f << "\" />" << endl;
    else if (endsWith(f,L".h"))
      file << "    <ClInclude Include=\"" << f << "\" />" << endl;
    else if (endsWith(f,L".asm"))
    {
      if (!_project->useNasm())
      {
        file << "    <CustomBuild Include=\"" << f << "\">" << endl;
        file << "      <Command>" << asmOptions() << "</Command>" << endl;
        file << "      <Outputs>$(IntDir)%(Filename).obj;%(Outputs)</Outputs>" << endl;
        file << "    </CustomBuild>" << endl;
      }
      else
      {
        folder=f.substr(0,f.find_last_of(L"\\"));
        file << "    <CustomBuild Include=\"" << f << "\">" << endl;
        file << "      <Command>" << nasmOptions(folder) << "</Command>" << endl;
        file << "      <Outputs>$(IntDir)%(Filename).obj;%(Outputs)</Outputs>" << endl;
        file << "    </CustomBuild>" << endl;
      }
    }
    else
    {
      fileName=f.substr(f.find_last_of(L"\\") + 1);

      count=1;
      if (fileCount.find(fileName) == fileCount.end())
        fileCount.insert(make_pair(fileName,count));
      else
        count=++fileCount[fileName];

      file << "    <ClCompile Include=\"" << f << "\">" << endl;
      name=replace(f,L"..\\",L"");
      if (contains(_cppFiles,name))
        file << "      <CompileAs>CompileAsCpp</CompileAs>" << endl;
      else if (count > 1)
      {
        name=f.substr(0,f.find_last_of(L"."));
        name=name.substr(name.find_last_of(L"\\") + 1);
        file << "      <ObjectFileName>$(IntDir)" << name << "_" << count << ".obj</ObjectFileName>" << endl;
      }
      file << "      <MultiProcessorCompilation>true</MultiProcessorCompilation>" << endl;
      file << "    </ClCompile>" << endl;
    }
  }
  file << "  </ItemGroup>" << endl;
}

void ProjectFile::writeFilter(wofstream &file) const
{
  wstring
    filter;

  vector<wstring>
    filters;

  file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
  file << "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">" << endl;
  file << "  <ItemGroup>" << endl;
  for (auto& f : _srcFiles)
  {
    wstring
      tagName(L"ClCompile");

    if (endsWith(f,L".asm"))
      tagName=L"CustomBuild";

    filter=getFilter(f,filters);
    if (filter != L"")
    {
      file << "    <" << tagName << L" Include=\"" << f << "\">" << endl;
      file << "      <Filter>" << filter << "</Filter>" << endl;
      file << "    </" << tagName << L">" << endl;
    }
    else
      file << "    <" << tagName << L" Include=\"" << f << "\" />" << endl;
  }
  file << "  </ItemGroup>" << endl;
  file << "  <ItemGroup>" << endl;
  for (auto& f : _includeFiles)
  {
    filter=getFilter(f,filters);
    if (filter != L"")
    {
      file << "    <CLInclude Include=\"" << f << "\">" << endl;
      file << "      <Filter>" << filter << "</Filter>" << endl;
      file << "    </CLInclude>" << endl;
    }
    else
      file << "    <CLInclude Include=\"" << f << "\" />" << endl;
  }
  file << "  </ItemGroup>" << endl;
  file << "  <ItemGroup>" << endl;
  for (auto& f : filters)
  {
    file << "    <Filter Include=\"" << f << "\">" << endl;
    file << "      <UniqueIdentifier>{" << createGuid() << "}</UniqueIdentifier>" << endl;
    file << "    </Filter>" << endl;
  }
  file << "  </ItemGroup>" << endl;
  file << "</Project>" << endl;
}

void ProjectFile::writeItemDefinitionGroup(wofstream &file,const bool debug,const vector<Project*> &allProjects) const
{
  wstring
    name;

  name=getTargetName(debug);

  file << "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='" << (debug ? "Debug" : "Release") << "|" << _wizard->platformName() << "'\">" << endl;
  file << "    <ClCompile>" << endl;
  file << "      <RuntimeLibrary>MultiThreaded" << (debug ? "Debug" : "") << (_wizard->solutionType() == SolutionType::STATIC_MT ? "" : "DLL") << "</RuntimeLibrary>" << endl;
  file << "      <StringPooling>true</StringPooling>" << endl;
  file << "      <FunctionLevelLinking>true</FunctionLevelLinking>" << endl;
  if (_project->warningLevel() == 0)
    file << "      <WarningLevel>TurnOffAllWarnings</WarningLevel>" << endl;
  else
    file << "      <WarningLevel>Level" << _project->warningLevel() << "</WarningLevel>" << endl;
  if (_project->treatWarningAsError())
    file << "      <TreatWarningAsError>true</TreatWarningAsError>" << endl;
  file << "      <SuppressStartupBanner>true</SuppressStartupBanner>" << endl;
  if (_project->compiler() == Compiler::CPP)
    file << "      <CompileAs>CompileAsCpp</CompileAs>" << endl;
  file << "      <InlineFunctionExpansion>" << (debug ? "Disabled" : "AnySuitable") << "</InlineFunctionExpansion>" << endl;
  file << "      <OpenMPSupport>" << (_wizard->useOpenMP() ? "true" : "false") << "</OpenMPSupport>" << endl;
  file << "      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>" << endl;
  file << "      <ProgramDatabaseFileName>" << binDirectory() << (_project->isExe() ? _name : name) << ".pdb</ProgramDatabaseFileName>" << endl;
  file << "      <BasicRuntimeChecks>" << (debug ? "EnableFastChecks" : "Default") << "</BasicRuntimeChecks>" << endl;
  file << "      <OmitFramePointers>" << (debug ? "false" : "true") << "</OmitFramePointers>" << endl;
  file << "      <Optimization>" << (debug || _project->isOptimizationDisable() ? "Disabled" : "MaxSpeed") << "</Optimization>" << endl;
  file << "      <AdditionalIncludeDirectories>";
  writeAdditionalIncludeDirectories(file,L";",allProjects);
  file << ";%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>" << endl;
  file << "      <PreprocessorDefinitions>";
  writePreprocessorDefinitions(file,debug);
  file << ";%(PreprocessorDefinitions)</PreprocessorDefinitions>" << endl;
  file << "      <AdditionalOptions>/source-charset:utf-8 %(AdditionalOptions)</AdditionalOptions>" << endl;
  file << "      <MultiProcessorCompilation>true</MultiProcessorCompilation>" << endl;
  file << "      <LanguageStandard>stdcpp17</LanguageStandard>" << endl;
  file << "      <LanguageStandard_C>stdc17</LanguageStandard_C>" << endl;
  file << "    </ClCompile>" << endl;
  file << "    <ResourceCompile>" << endl;
  file << "      <PreprocessorDefinitions>" << (debug ? "_DEBUG" : "NDEBUG") <<";%(PreprocessorDefinitions)</PreprocessorDefinitions>" << endl;
  file << "      <Culture>0x0409</Culture>" << endl;
  file << "    </ResourceCompile>" << endl;

  if (isLib())
  {
    file << "    <Lib>" << endl;
    file << "      <AdditionalLibraryDirectories>" << libDirectory() << ";%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>" << endl;
    file << "      <AdditionalDependencies>/MACHINE:" << (_wizard->machineName());
    writeAdditionalDependencies(file,L";");
    file << ";%(AdditionalDependencies)</AdditionalDependencies>" << endl;
    file << "      <SuppressStartupBanner>true</SuppressStartupBanner>" << endl;
    file << "    </Lib>" << endl;
  }
  else
  {
    file << "    <Link>" << endl;
    file << "      <AdditionalLibraryDirectories>" << libDirectory() << ";%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>" << endl;
    file << "      <AdditionalDependencies>/MACHINE:" << _wizard->machineName();
    writeAdditionalDependencies(file,L";");
    file << ";%(AdditionalDependencies)</AdditionalDependencies>" << endl;
    file << "      <SuppressStartupBanner>true</SuppressStartupBanner>" << endl;
    file << "      <TargetMachine>Machine" << _wizard->machineName() << "</TargetMachine>" << endl;
    file << "      <GenerateDebugInformation>" << (debug ? "true" : "false") << "</GenerateDebugInformation>" << endl;
    file << "      <ProgramDatabaseFile>" << binDirectory() << (_project->isExe() ? _name : name) << ".pdb</ProgramDatabaseFile>" << endl;
    file << "      <ImportLibrary>" << libDirectory() << name << ".lib</ImportLibrary>" << endl;
    if (!_project->isConsole())
    {
      if (_project->isDll())
        file << "      <LinkDLL>true</LinkDLL>" << endl;
      else if (_project->useUnicode())
        file << "    <EntryPointSymbol>wWinMainCRTStartup</EntryPointSymbol>" << endl;
      file << "      <SubSystem>Windows</SubSystem>" << endl;
      if ((_project->isDll()) && (!_project->moduleDefinitionFile().empty()))
        file << "      <ModuleDefinitionFile>" << rootPath <<  _project->filePath(_project->moduleDefinitionFile()) << "</ModuleDefinitionFile>" << endl;
    }
    else
      file << "      <SubSystem>Console</SubSystem>" << endl;
    file << "    </Link>" << endl;
  }
  file << "  </ItemDefinitionGroup>" << endl;
}

void ProjectFile::writePreprocessorDefinitions(wofstream &file,const bool debug) const
{
  file << (debug ? "_DEBUG" : "NDEBUG") << ";_WINDOWS;WIN32;_VISUALC_;NeedFunctionPrototypes;_WIN32_WINNT=0x0601";
  for (auto& def : _project->defines())
  {
    file << ";" << def;
  }
  if (isLib() || (_wizard->solutionType() != SolutionType::DYNAMIC_MT && (_project->isExe())))
  {
    for (auto& def : _definesLib)
    {
      file << ";" << def;
    }
    file << ";_LIB";
  }
  else if (_project->isDll())
  {
    for (auto& def : _project->definesDll())
    {
      file << ";" << def;
    }
    file << ";_DLL;_MAGICKMOD_";
  }
  if (_project->isExe() && _wizard->solutionType() != SolutionType::STATIC_MT)
    file << ";_AFXDLL";
  if (_wizard->includeIncompatibleLicense())
    file << ";_MAGICK_INCOMPATIBLE_LICENSES_";
}

void ProjectFile::writeProjectReferences(wofstream &file,const vector<Project*> &allProjects) const
{
  size_t
    index;

  wstring
    projectName,
    projectFileName;

  file << "  <ItemGroup>" << endl;

  for (auto& dep : _dependencies)
  {
    projectName=dep;
    projectFileName=L"";
    index=dep.find(L">");
    if (index != -1)
    {
      projectName=dep.substr(0,index);
      projectFileName=dep.substr(index+1);
    }

    for (auto& depp : allProjects)
    {
      if (depp->name() != projectName)
        continue;

      for (auto& deppf : depp->files())
      {
        if (projectFileName != L"" && deppf->_name != projectFileName)
          continue;

        file << "    <ProjectReference Include=\"..\\" << deppf->name() << "\\" << deppf->_fileName << "\">" << endl;
        file << "      <Project>{" << deppf->guid() << "}</Project>" << endl;
        file << "      <ReferenceOutputAssembly>false</ReferenceOutputAssembly>" << endl;
        file << "    </ProjectReference>" << endl;
      }
    }
  }

  file << "  </ItemGroup>" << endl;
}
