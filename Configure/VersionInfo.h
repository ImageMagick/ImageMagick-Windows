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

  const wstring fullVersion() const;

  const wstring gitRevision() const;

  const wstring interfaceVersion() const;

  const wstring libAddendum() const;

  const wstring libVersion() const;

  const wstring libVersionNumber() const;

  bool load();

  const wstring majorVersion() const;

  const wstring ppLibVersionNumber() const;

  const wstring ppInterfaceVersion() const;

  const wstring releaseDate() const;

  const wstring version() const;

  const wstring versionNumber() const;

private:

  const wstring executeCommand(const wstring &command) const;

  const wstring getFileModificationDate(const wstring &fileName,const wstring &format) const;

  void loadValue(const wstring &line,const wstring &keyword,wstring *value) const;

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
