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
#ifndef __Project__
#define __Project__

#include "ConfigureWizard.h"
#include "ProjectFile.h"
#include "Shared.h"

class Project
{
public:

  Compiler compiler(VisualStudioVersion visualStudioVersion) const;

  const wstring configDefine() const;

  const vector<wstring> &defines();

  const vector<wstring> &definesDll();

  const vector<wstring> &definesLib();

  const vector<wstring> &dependencies();

  const vector<wstring> &directories();

  const vector<wstring> &excludes();

  const vector<ProjectFile*> &files() const;

  const vector<wstring> &includes();

  const vector<wstring> &includesNasm();

  const vector<wstring> &platformExcludes(Platform platform);

  const wstring configPath(const wstring &subPath) const;

  const wstring filePath(const wstring &subPath) const;

  bool isConsole() const;

  bool isDll() const;

  bool isExe() const;

  bool isFuzz() const;

  bool isLib() const;

  bool isModule() const;

  bool isOptimizationDisable() const;

  bool isSupported(const VisualStudioVersion visualStudioVersion) const;

  const vector<wstring> &libraries();

  const wstring moduleDefinitionFile() const;

  const wstring name() const;

  const wstring notice() const;

  const vector<wstring> &references();

  bool treatWarningAsError() const;

  bool useNasm() const;

  bool useOpenCL() const;

  bool useUnicode() const;

  const wstring version() const;

  int warningLevel() const;

  void checkFiles(const VisualStudioVersion visualStudioVersion);

  static Project* create(const wstring &configFolder,const wstring &filesFolder,const wstring& name);

  bool loadFiles(const ConfigureWizard &wizard);

  void mergeProjectFiles(const ConfigureWizard &wizard);

  bool shouldSkip(const ConfigureWizard &wizard) const;

private:
  Project(const wstring &configFolder,const wstring &filesFolder,const wstring &name);

  void addLines(wifstream &config,wstring &value);

  void addLines(wifstream &config,vector<wstring> &container);

  void loadConfig(wifstream &config);

  void loadModules(const ConfigureWizard &wizard);

  const vector<wstring> readLicenseFilenames(const wstring &line) const;

  void setNoticeAndVersion();

  wstring              _configDefine;
  wstring              _configFolder;
  vector<wstring>      _defines;
  vector<wstring>      _definesDll;
  vector<wstring>      _definesLib;
  vector<wstring>      _dependencies;
  vector<wstring>      _directories;
  bool                 _disabledARM64;
  bool                 _disableOptimization;
  vector<wstring>      _excludes;
  vector<wstring>      _excludesX86;
  vector<wstring>      _excludesX64;
  vector<wstring>      _excludesARM64;
  vector<ProjectFile*> _files;
  wstring              _filesFolder;
  bool                 _hasIncompatibleLicense;
  vector<wstring>      _includes;
  vector<wstring>      _includesNasm;
  bool                 _isOptional;
  vector<wstring>      _libraries;
  vector<wstring>      _licenseFileNames;
  bool                 _magickProject;
  VisualStudioVersion  _minimumVisualStudioVersion;
  wstring              _moduleDefinitionFile;
  wstring              _modulePrefix;
  wstring              _name;
  wstring              _notice;
  wstring              _path;
  vector<wstring>      _references;
  ProjectType          _type;
  bool                 _useNasm;
  bool                 _useOpenCL;
  bool                 _useUnicode;
  vector<wstring>      _versions;
};

#endif // __Project__
