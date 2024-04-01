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

Compiler Project::compiler(VisualStudioVersion visualStudioVersion) const
{
  return(_magickProject && visualStudioVersion >= VisualStudioVersion::VS2022 ? Compiler::CPP : Compiler::Default);
}

const wstring Project::configDefine() const
{
  return(_configDefine);
}

const vector<wstring> &Project::defines()
{
  return(_defines);
}

const vector<wstring> &Project::definesDll()
{
  return(_definesDll);
}

const vector<wstring> &Project::definesLib()
{
  return(_definesLib);
}

const vector<wstring> &Project::dependencies()
{
  return(_dependencies);
}

const vector<wstring> &Project::directories()
{
  return(_directories);
}

const vector<wstring> &Project::excludes()
{
  return(_excludes);
}

const vector<ProjectFile*> &Project::files() const
{
  return(_files);
}

const vector<wstring> &Project::includes()
{
  return(_includes);
}

const vector<wstring> &Project::includesNasm()
{
  return(_includesNasm);
}

const vector<wstring> &Project::platformExcludes(Platform platform)
{
  switch (platform)
  {
    case Platform::X86: return(_excludesX86);
    case Platform::X64: return(_excludesX64);
    case Platform::ARM64: return(_excludesARM64);
    default: throw;
  }
}

const wstring Project::configPath(const wstring &subPath) const
{
  return(_configFolder + L"\\" + subPath);
}

const wstring Project::filePath(const wstring &subPath) const
{
  wstring
    path = _filesFolder + L"\\";

  path += _path != L"" ? _path : _name;

  if (subPath == L".")
    return(path);

  if (subPath == L"..")
    return( _filesFolder + L"\\");

  return(path + L"\\" + subPath);
}

bool Project::isConsole() const
{
  if (!isExe())
    return(false);
  return(_type != ProjectType::APPTYPE);
}

bool Project::isDll() const
{
  return((_type == ProjectType::DLLTYPE) || (_type == ProjectType::DLLMODULETYPE));
}

bool Project::isExe() const
{
  return((_type == ProjectType::EXETYPE) || (_type == ProjectType::EXEMODULETYPE) || (_type == ProjectType::APPTYPE));
}

bool Project::isFuzz() const
{
  return(_modulePrefix == L"FUZZ");
}

bool Project::isLib() const
{
  return((_type == ProjectType::STATICTYPE));
}

bool Project::isModule() const
{
  return((_type == ProjectType::DLLMODULETYPE) || (_type == ProjectType::EXEMODULETYPE));
}

bool Project::isOptimizationDisable() const
{
  return(_disableOptimization);
}

bool Project::isSupported(const VisualStudioVersion visualStudioVersion) const
{
  return(visualStudioVersion >= _minimumVisualStudioVersion);
}

const vector<wstring> &Project::libraries()
{
  return(_libraries);
}

const wstring Project::moduleDefinitionFile() const
{
  return(_moduleDefinitionFile);
}

const wstring Project::name() const
{
  return(_name);
}

const wstring Project::notice() const
{
  return(_notice);
}

const vector<wstring> &Project::references()
{
  return(_references);
}

bool Project::treatWarningAsError() const
{
  return(_magickProject && _wizard.isImageMagick7());
}

bool Project::useNasm() const
{
  return(_useNasm);
};

bool Project::useOpenCL() const
{
  return(_useOpenCL);
}

bool Project::useUnicode() const
{
  return(_useUnicode);
}

const wstring Project::version() const
{
  return _versions[0];
}

int Project::warningLevel() const
{
  return(_magickProject && _wizard.isImageMagick7() ? 4 : 0);
}

void Project::checkFiles(const VisualStudioVersion visualStudioVersion)
{
   std::vector<ProjectFile*>
     newFiles(_files.size());

  auto filter=[visualStudioVersion](ProjectFile* p){ return p->isSupported(visualStudioVersion); };
  auto it=std::copy_if(_files.begin(),_files.end(),newFiles.begin(),filter);
  newFiles.resize(std::distance(newFiles.begin(),it));
  _files=newFiles;
}

void Project::mergeProjectFiles()
{
  ProjectFile
    *projectFile;

  if ((_type != ProjectType::DLLMODULETYPE) || (_wizard.solutionType() == SolutionType::DYNAMIC_MT))
    return;

  projectFile=new ProjectFile(&_wizard,this,L"CORE",_name);
  for (auto& file : _files)
  {
    projectFile->merge(file);
  }
  _files.clear();
  _files.push_back(projectFile);
}

Project* Project::create(const ConfigureWizard &wizard,const wstring &configFolder, const wstring &filesFolder, const wstring &name)
{
  wifstream
    config;

  wstring
    configPath;

  configPath=configFolder + L"\\" + name + L"\\.ImageMagick";
  config.open(pathFromRoot(configPath + L"\\Config.txt"));
  if (!config)
  {
    configPath=configFolder + L"\\" + name;
    config.open(pathFromRoot(configPath +L"\\Config.txt"));
  }

  if (!config)
    return((Project *) NULL);

  Project* project = new Project(wizard,configPath,filesFolder,name);
  project->loadConfig(config);
  config.close();

  if (project->_onlyImageMagick7 && !wizard.isImageMagick7())
    return((Project *) NULL);

  project->setNoticeAndVersion();

  return(project);
}

bool Project::loadFiles()
{
  ProjectFile
    *projectFile;

  _files.clear();

  if (shouldSkip())
    return(false);

  switch(_type)
  {
    case ProjectType::DLLMODULETYPE:
    {
      loadModules();
      break;
    }
    case ProjectType::DLLTYPE:
    {
      projectFile=new ProjectFile(&_wizard,this,L"CORE",_name);
      _files.push_back(projectFile);
      break;
    }
    case ProjectType::APPTYPE:
    case ProjectType::EXETYPE:
    {
      projectFile=new ProjectFile(&_wizard,this,L"UTIL",_name);
      _files.push_back(projectFile);
      break;
    }
    case ProjectType::EXEMODULETYPE:
    {
      loadModules();
      break;
    }
    case ProjectType::STATICTYPE:
    {
      projectFile=new ProjectFile(&_wizard,this,L"CORE",_name);
      _files.push_back(projectFile);
      break;
    }
  }

  return(true);
}

bool Project::shouldSkip() const
{
  if (_disabledARM64 && _wizard.platform() == Platform::ARM64)
    return(true);

  if (_hasIncompatibleLicense && !_wizard.includeIncompatibleLicense())
    return(true);

  if (_isOptional && !_wizard.includeOptional())
    return(true);

  return(false);
}

void Project::updateProjectNames()
{
  _wizard.updateProjectNames(_name);

  updateProjectNames(_dependencies);
  updateProjectNames(_directories);
  updateProjectNames(_includes);
  updateProjectNames(_references);
}

void Project::updateProjectNames(vector<wstring> &vector)
{
  for (auto& value : vector)
    _wizard.updateProjectNames(value);
}

Project::Project(const ConfigureWizard &wizard,const wstring &configFolder,const wstring &filesFolder,const wstring &name)
  : _wizard(wizard)
{
  _configFolder=configFolder;
  _filesFolder=filesFolder;
  _name=name;

  _disabledARM64=false;
  _disableOptimization=false;
  _hasIncompatibleLicense=false;
  _isOptional=false;
  _magickProject=false;
  _minimumVisualStudioVersion=VSEARLIEST;
  _onlyImageMagick7=false;
  _type=ProjectType::UNDEFINEDTYPE;
  _useNasm=false;
  _useOpenCL=false;
  _useUnicode=false;
}

void Project::addLines(wifstream &config,wstring &value)
{
  wstring
    line;

  while (!config.eof())
  {
    getline(config,line);
    if (trim(line).empty())
      return;

    value+=line+L"\n";
  }
}

void Project::addLines(wifstream &config,vector<wstring> &container)
{
  wstring
    line;

  while (!config.eof())
  {
    line=readLine(config);
    if (line.empty())
      return;

    container.push_back(line);
  }
}

void Project::loadConfig(wifstream &config)
{
  wstring
    line;

  while (!config.eof())
  {
    line=readLine(config);
    if (line == L"[APP]")
      _type=ProjectType::APPTYPE;
    else if (line == L"[CONFIG_DEFINE]")
      addLines(config,_configDefine);
    else if (line == L"[DEFINES_DLL]")
      addLines(config,_definesDll);
    else if (line == L"[DEFINES_LIB]")
      addLines(config,_definesLib);
    else if (line == L"[DEFINES]")
      addLines(config,_defines);
    else if (line == L"[DEPENDENCIES]")
      addLines(config,_dependencies);
    else if (line == L"[DIRECTORIES]")
      addLines(config,_directories);
    else if (line == L"[DISABLED_ARM64]")
      _disabledARM64=true;
    else if (line == L"[DISABLE_OPTIMIZATION]")
      _disableOptimization=true;
    else if (line == L"[DLL]")
      _type=ProjectType::DLLTYPE;
    else if (line == L"[DLLMODULE]")
      _type=ProjectType::DLLMODULETYPE;
    else if (line == L"[EXE]")
      _type=ProjectType::EXETYPE;
    else if (line == L"[EXEMODULE]")
      _type=ProjectType::EXEMODULETYPE;
    else if (line == L"[EXCLUDES]")
      addLines(config,_excludes);
    else if (line == L"[EXCLUDES_X86]")
      addLines(config,_excludesX86);
    else if (line == L"[EXCLUDES_X64]")
      addLines(config,_excludesX64);
    else if (line == L"[EXCLUDES_ARM64]")
      addLines(config,_excludesARM64);
    else if (line == L"[INCLUDES]")
      addLines(config,_includes);
    else if (line == L"[INCLUDES_NASM]")
      addLines(config,_includesNasm);
    else if (line == L"[INCOMPATIBLE_LICENSE]")
      _hasIncompatibleLicense=true;
    else if (line == L"[STATIC]")
      _type=ProjectType::STATICTYPE;
    else if (line == L"[LIBRARIES]")
      addLines(config,_libraries);
    else if (line == L"[MODULE_DEFINITION_FILE]")
      _moduleDefinitionFile=readLine(config);
    else if (line == L"[MODULE_PREFIX]")
      _modulePrefix=readLine(config);
    else if (line == L"[NASM]")
      _useNasm=true;
    else if (line == L"[ONLY_IMAGEMAGICK7]")
      _onlyImageMagick7=true;
    else if (line == L"[OPTIONAL]")
      _isOptional=true;
    else if (line == L"[PATH]")
      _path=readLine(config);
    else if (line == L"[REFERENCES]")
      addLines(config,_references);
    else if (line == L"[UNICODE]")
      _useUnicode=true;
    else if (line == L"[OPENCL]")
      _useOpenCL=true;
    else if (line == L"[VISUAL_STUDIO]")
      _minimumVisualStudioVersion=parseVisualStudioVersion(readLine(config));
    else if (line == L"[MAGICK_PROJECT]")
      _magickProject=true;
    else if (line == L"[LICENSE]")
      _licenseFileNames=readLicenseFilenames(readLine(config));
  }
}

void Project::loadModules()
{
  ProjectFile
    *projectAlias,
    *projectFile;

  for (auto& dir :_directories)
  {
    const wstring
      path(pathFromRoot(filePath(dir)));

    if (!directoryExists(path))
      throwException(L"Invalid folder specified: " + path);
    
    for (const auto& entry : filesystem::directory_iterator(path))
    {
      wstring
        fileName,
        name;

      if (!entry.is_regular_file())
        continue;

      fileName=entry.path().filename();
      if (contains(_excludes,fileName) || startsWith(fileName,L"main.") || !isValidSrcFile(fileName))
        continue;

      name=fileName;
      name=name.substr(0,name.find_last_of(L"."));
      projectFile=new ProjectFile(&_wizard,this,_modulePrefix,name);
      _files.push_back(projectFile);

      for (auto& alias : projectFile->aliases())
      {
        projectAlias=new ProjectFile(&_wizard,this,_modulePrefix,alias,name);
        _files.push_back(projectAlias);
      }
    }
  }
}

const vector<wstring> Project::readLicenseFilenames(const wstring &line) const
{
  wstring
    fileName;

  vector<wstring>
    fileNames;

  wstringstream
    wss(line);

  while(getline(wss, fileName, L';'))
  {
    wstring
      filePath(pathFromRoot(filePath(fileName)));

    filesystem::path
      file(filePath);

    if (!filesystem::exists(file))
      throwException(L"Unable to open license file: " + fileName);

    fileNames.push_back(filePath);
  }

  return (fileNames);
}

void Project::setNoticeAndVersion()
{
  _notice=L"";
  for (auto& licenseFileName : _licenseFileNames)
  {
    filesystem::path
      folder,
      versionFile;

    wifstream
      version;

    wstring
      versionFileName;

    folder=filesystem::path(licenseFileName).parent_path();
    versionFileName=folder.wstring()+L"\\.ImageMagick\\ImageMagick.version.h";
    versionFile=filesystem::path(versionFileName).wstring();
    if (!filesystem::exists(versionFile))
      {
        folder=folder.parent_path();
        versionFileName=folder.wstring()+L"\\.ImageMagick\\ImageMagick.version.h";
        versionFile=filesystem::path(versionFileName).wstring();
        if (!filesystem::exists(versionFile))
          throwException(L"Unable to find version file for: " + _name);
      }

    version.open(versionFileName);
    while (!version.eof())
    {
      wstring
        line;

      line=readLine(version);
      if (!startsWith(line,L"DELEGATE_VERSION_NUM"))
        continue;

      line=line.substr(line.find_last_of(L" "));
      line=replace(line,L",",L".");
      _versions.push_back(line);
      break;
    }
    version.close();

    _notice+=L"[ "+folder.stem().wstring()+_versions.back()+L" ] copyright:\r\n";
    _notice+=readLicense(licenseFileName)+L"\r\n";
  }
}
