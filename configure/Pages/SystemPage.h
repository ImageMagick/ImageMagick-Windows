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
#ifndef __SystemPage__
#define __SystemPage__

class SystemPage : public CPropertyPage
{
  DECLARE_DYNCREATE(SystemPage)

public:

  SystemPage();

  ~SystemPage();

  wstring binDirectory() const;

  wstring fuzzBinDirectory() const;

  wstring libDirectory() const;

  virtual BOOL OnSetActive();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);

  afx_msg void OnBinDirBrowse();

  afx_msg void OnFuzzBinDirBrowse();

  virtual BOOL OnInitDialog();

  afx_msg void OnLibDirBrowse();

  DECLARE_MESSAGE_MAP()

private:

  wstring addBackslash(const CStringW &directory) const;

  bool browseForFolder(const wchar_t *title, wchar_t *retval);

  void setDirectory(const wstring &title,CStringW &directory);

  CStringW _binDirectory;
  CStringW _fuzzBinDirectory;
  CStringW _libDirectory;
};

#endif // __SystemPage__
