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
#ifndef __ProjectFile__
#define __ProjectFile__

#include "ConfigureWizard.h"

class Project;

class ProjectFile
{
public:
  ProjectFile(const ConfigureWizard *wizard,Project *project,
    const wstring &prefix,const wstring &name);

  ProjectFile(const ConfigureWizard *wizard,Project *project,
    const wstring &prefix,const wstring &name,const wstring &reference);

  const vector<wstring> &dependencies() const;

  const wstring fileName() const;

  const wstring guid() const;

  const wstring name() const;

  const vector<wstring> &aliases() const;

  bool isSupported(const VisualStudioVersion visualStudioVersion) const;

  void loadConfig();

  void merge(ProjectFile *projectFile);

  void write(const vector<Project*> &allProjects);

private:

  const wstring binDirectory() const;

  bool isLib() const;

  const wstring libDirectory() const;

  const wstring outputDirectory() const;

  void addFile(const wstring &name);

  void addLines(wifstream &config,vector<wstring> &container);

  const wstring asmOptions() const;

  const wstring getFilter(const wstring &fileName,vector<wstring> &filters) const;

  const wstring getIntermediateDirectoryName(const bool debug) const;

  const wstring getTargetName(const bool debug) const;

  void initialize(Project* project);

  bool isSrcFile(const wstring &fileName);

  bool isExcluded(const wstring &fileName);

  void loadAliases();

  void loadModule();

  void loadSource();

  void loadSource(const wstring &directory);

  const wstring nasmOptions(const wstring &folder) const;

  void merge(vector<wstring> &input, vector<wstring> &output);

  void setFileName();

  void write(wofstream &file,const vector<Project*> &allProjects) const;

  void writeFiles(wofstream &file,const vector<wstring> &collection) const;

  void writeFilter(wofstream &file) const;

  void writeAdditionalDependencies(wofstream &file,const wstring &separator) const;

  void writeAdditionalIncludeDirectories(wofstream &file,const wstring &separator,const vector<Project*> &allProjects) const;

  void writeItemDefinitionGroup(wofstream &file,const bool debug,const vector<Project*> &allProjects) const;

  void writePreprocessorDefinitions(wofstream &file,const bool debug) const;

  void writeProjectReferences(wofstream &file,const vector<Project*> &allProjects) const;

  vector<wstring>        _aliases;
  vector<wstring>        _cppFiles;
  vector<wstring>        _dependencies;
  wstring                _fileName;
  wstring                _guid;
  vector<wstring>        _includeFiles;
  vector<wstring>        _includes;
  vector<wstring>        _definesLib;
  VisualStudioVersion    _minimumVisualStudioVersion;
  wstring                _name;
  wstring                _prefix;
  Project               *_project;
  wstring                _reference;
  vector<wstring>        _resourceFiles;
  vector<wstring>        _srcFiles;
  const ConfigureWizard *_wizard;
};

#endif // __ProjectFile__
