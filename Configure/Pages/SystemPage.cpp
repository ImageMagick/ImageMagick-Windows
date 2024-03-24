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
#include "..\stdafx.h"
#include "..\ConfigureApp.h"
#include "SystemPage.h"
#include <Shldisp.h>

IMPLEMENT_DYNCREATE(SystemPage, CPropertyPage)

SystemPage::SystemPage() : CPropertyPage(IDD_SYSTEM_PAGE)
{
  _binDirectory=L"Output\\ImageMagick\\bin\\";
  _fuzzBinDirectory="Output\\Fuzz\\bin\\";
  _libDirectory="Output\\ImageMagick\\lib\\";
}

SystemPage::~SystemPage()
{
}

wstring SystemPage::binDirectory() const
{
  return(addBackslash(_binDirectory));
}

wstring SystemPage::fuzzBinDirectory() const
{
  return(addBackslash(_fuzzBinDirectory));
}

wstring SystemPage::libDirectory() const
{
  return(addBackslash(_libDirectory));
}

BOOL SystemPage::OnSetActive()
{
  CPropertySheet
    *propertySheet;

  propertySheet=(CPropertySheet*) GetParent();
  propertySheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

  return(CPropertyPage::OnSetActive());
}

void SystemPage::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Text(pDX,IDC_BIN_DIR,_binDirectory);
  DDX_Text(pDX,IDC_FUZZ_BIN_DIR,_fuzzBinDirectory);
  DDX_Text(pDX,IDC_LIB_DIR,_libDirectory);
}

void SystemPage::OnBinDirBrowse()
{
  setDirectory(L"Select Bin directory",_binDirectory);
}

void SystemPage::OnFuzzBinDirBrowse()
{
  setDirectory(L"Select Fuzz Bin directory",_fuzzBinDirectory);
}

BOOL SystemPage::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();

  UpdateData(FALSE);

  return(TRUE);
}

void SystemPage::OnLibDirBrowse()
{
  setDirectory(L"Select Lib directory",_libDirectory);
}

BEGIN_MESSAGE_MAP(SystemPage, CPropertyPage)
  ON_BN_CLICKED(IDC_BIN_DIR, OnBinDirBrowse)
  ON_BN_CLICKED(IDC_FUZZ_BIN_DIR, OnFuzzBinDirBrowse)
  ON_BN_CLICKED(IDC_BIN_DIR_BROWSE, OnLibDirBrowse)
END_MESSAGE_MAP()

wstring SystemPage::addBackslash(const CStringW &directory) const
{
  if (directory.GetAt(directory.GetLength()-1) != '\\')
    return(wstring(directory) + L"\\");
  else
    return(wstring(directory));
}

bool SystemPage::browseForFolder(const wchar_t *title, wchar_t *retval)
{
  wchar_t
    directory[_MAX_PATH];

  BROWSEINFO
    info;

  LPITEMIDLIST
    itemidlist;

  LPMALLOC
    pMalloc;

  memset(directory,'\0',_MAX_PATH);

  if (::SHGetMalloc(&pMalloc) != NOERROR)
    return(false);

  info.hwndOwner = m_hWnd;
  info.pidlRoot = NULL;
  info.pszDisplayName = directory;
  info.lpszTitle = title;
  info.ulFlags = 0;
  info.lpfn = NULL;

  itemidlist = SHBrowseForFolder(&info);
  if (itemidlist == NULL) // User clicked Cancel
  {
    pMalloc->Release();
    return(false);
  }

  SHGetPathFromIDList(itemidlist, retval);
  pMalloc->Free(itemidlist);
  pMalloc->Release();
  return(true);
}

void SystemPage::setDirectory(const wstring &title,CStringW &directory)
{
  wchar_t
    path[_MAX_PATH];

  UpdateData(TRUE);
  if (browseForFolder(title.c_str(), path))
    directory=path;
  UpdateData(FALSE);
}

