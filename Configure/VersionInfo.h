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
#ifndef __VersionInfo__
#define __VersionInfo__

class VersionInfo
{
public:

  VersionInfo();

  wstring fullVersion() const;

  wstring gitRevision() const;

  wstring interfaceVersion() const;

  wstring libAddendum() const;

  wstring libVersion() const;

  wstring libVersionNumber() const;

  bool load();

  wstring majorVersion() const;

  wstring ppLibVersionNumber() const;

  wstring ppInterfaceVersion() const;

  wstring releaseDate() const;

  wstring version() const;

  wstring versionNumber() const;

private:

  wstring executeCommand(const wchar_t *command);

  wstring getFileModificationDate(const wchar_t *fileName,const wchar_t *format);

  void loadValue(const wstring line,const wstring keyword,wstring *value);

  void setGitRevision();

  void setReleaseDate();

  wstring _gitRevision;
  wstring _isBeta;
  wstring _libraryCurrent;
  wstring _libraryRevision;
  wstring _libraryAge;
  wstring _libVersion;
  wstring _major;
  wstring _minor;
  wstring _micro;
  wstring _patchlevel;
  wstring _ppLibraryCurrent;
  wstring _ppLibraryRevision;
  wstring _ppLibraryAge;
  wstring _releaseDate;
};

#endif // __VersionInfo__
