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
#ifndef __Solution__
#define __Solution__

#include "Project.h"
#include "ConfigureWizard.h"
#include "VersionInfo.h"
#include "WaitDialog.h"

class Solution
{
public:
  Solution(const ConfigureWizard &wizard);

  int loadProjectFiles();

  void loadProjects();

  void write(WaitDialog &waitDialog);

private:

  void checkKeyword(const wstring keyword);

  wstring getFileName();

  wstring getMagickFolderName();

  void loadProjectsFromFolder(const wstring &folder, const wstring &filesFolder);

  void writeMagickBaseConfig();

  void writeMakeFile();

  void writeNotice(const VersionInfo &versionInfo);

  void writePolicyConfig();

  void writeThresholdMap();

  void writeVersion(const VersionInfo &versionInfo);

  void writeVersion(const VersionInfo &versionInfo,wstring input,wstring output);

  void write(wofstream &file);

  vector<Project*> _projects;
  const ConfigureWizard &_wizard;
};

#endif // __Solution__
