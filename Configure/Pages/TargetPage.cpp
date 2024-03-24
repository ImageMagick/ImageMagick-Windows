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
#include "TargetPage.h"

IMPLEMENT_DYNCREATE(TargetPage, CPropertyPage)

TargetPage::TargetPage() : CPropertyPage(IDD_TARGET_PAGE)
{
  setVisualStudioVersion();

#if _M_IX86
  _platform=Platform::X86;
#elif _M_ARM64
  _platform=Platform::ARM64;
#else
  _platform=Platform::X64;
#endif
  _enableDpc=TRUE;
  _excludeAliases=FALSE;
  _excludeDeprecated=TRUE;
#ifdef DEBUG
  _includeIncompatibleLicense=TRUE;
  _includeOptional=TRUE;
#else
  _includeIncompatibleLicense=FALSE;
  _includeOptional=FALSE;
#endif
  _installedSupport=FALSE;
  _policyConfig=PolicyConfig::OPEN;
  _quantumDepth=QuantumDepth::Q16;
  _solutionType=SolutionType::DYNAMIC_MT;
  _useHDRI=PathFileExists(pathFromRoot(L"VisualMagick\\MagickCore").c_str()) ? TRUE : FALSE;
  _useOpenCL=TRUE;
  _useOpenMP=TRUE;
  _zeroConfigurationSupport=FALSE;
}

TargetPage::~TargetPage()
{
}

bool TargetPage::enableDpc() const
{
  return(_enableDpc == TRUE);
}

void TargetPage::enableDpc(bool value)
{
  _enableDpc=value;
}

bool TargetPage::excludeAliases() const
{
  return(_excludeAliases == TRUE);
}

void TargetPage::excludeAliases(bool value)
{
  _excludeAliases=value;
}

bool TargetPage::excludeDeprecated() const
{
  return(_excludeDeprecated == TRUE);
}

void TargetPage::excludeDeprecated(bool value)
{
  _excludeDeprecated=value;
}

bool TargetPage::includeIncompatibleLicense() const
{
  return(_includeIncompatibleLicense == TRUE);
}

void TargetPage::includeIncompatibleLicense(bool value)
{
  _includeIncompatibleLicense=value;
}

bool TargetPage::includeOptional() const
{
  return(_includeOptional == TRUE);
}

void TargetPage::includeOptional(bool value)
{
  _includeOptional=value;
}

bool TargetPage::installedSupport() const
{
  return(_installedSupport == TRUE);
}

void TargetPage::installedSupport(bool value)
{
  _installedSupport=value;
}

Platform TargetPage::platform() const
{
  return(_platform);
}

void TargetPage::platform(Platform value)
{
  _platform=value;
}

PolicyConfig TargetPage::policyConfig() const
{
  return(_policyConfig);
}

void TargetPage::policyConfig(PolicyConfig value)
{
  _policyConfig=value;
}

QuantumDepth TargetPage::quantumDepth() const
{
  return(_quantumDepth);
}

void TargetPage::quantumDepth(QuantumDepth value)
{
  _quantumDepth=value;
}

SolutionType TargetPage::solutionType() const
{
  return(_solutionType);
}

void TargetPage::solutionType(SolutionType value)
{
  _solutionType=value;
}

bool TargetPage::useHDRI() const
{
  return(_useHDRI == TRUE);
}

void TargetPage::useHDRI(bool value)
{
  _useHDRI=value;
}

bool TargetPage::useOpenCL() const
{
  return(_useOpenCL == TRUE);
}

void TargetPage::useOpenCL(bool value)
{
  _useOpenCL=value;
}

bool TargetPage::useOpenMP() const
{
  return(_useOpenMP == TRUE);
}

void TargetPage::useOpenMP(bool value)
{
  _useOpenMP=value;
}

VisualStudioVersion TargetPage::visualStudioVersion() const
{
  return(_visualStudioVersion);
}

void TargetPage::visualStudioVersion(VisualStudioVersion value)
{
  _visualStudioVersion=value;
}

bool TargetPage::zeroConfigurationSupport() const
{
  return(_zeroConfigurationSupport != FALSE);
}

void TargetPage::zeroConfigurationSupport(bool value)
{
  _zeroConfigurationSupport=value;
}

void TargetPage::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);

  DDX_CBIndex(pDX,IDC_QUANTUM_DEPTH,(int&) _quantumDepth);
  DDX_CBIndex(pDX,IDC_PLATFORM,(int&) _platform);
  DDX_CBIndex(pDX,IDC_VISUALSTUDIO,(int&) _visualStudioVersion);
  DDX_CBIndex(pDX,IDC_POLICYCONFIG,(int&) _policyConfig);
  DDX_Radio(pDX,IDC_PROJECT_DYNAMIC_MT,(int&) _solutionType);
  DDX_Check(pDX,IDC_HDRI,_useHDRI);
  DDX_Check(pDX,IDC_OPEN_MP,_useOpenMP);
  DDX_Check(pDX,IDC_OPEN_CL,_useOpenCL);
  DDX_Check(pDX,IDC_ENABLE_DPC,_enableDpc);
  DDX_Check(pDX,IDC_INCLUDE_INCOMPATIBLE,_includeIncompatibleLicense);
  DDX_Check(pDX,IDC_INCLUDE_OPTIONAL,_includeOptional);
  DDX_Check(pDX,IDC_EXCLUDE_DEPRECATED,_excludeDeprecated);
  DDX_Check(pDX,IDC_INSTALLED_SUPPORT,_installedSupport);
  DDX_Check(pDX,IDC_ZERO_CONFIGURATION_SUPPORT,_zeroConfigurationSupport);
}

BOOL TargetPage::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  UpdateData(FALSE);

  return TRUE;
}

BEGIN_MESSAGE_MAP(TargetPage, CPropertyPage)
END_MESSAGE_MAP()

wstring TargetPage::getEnvironmentVariable(const wchar_t *name)
{
  wchar_t
    *buffer;

  size_t
    length;

  wstring
    value;

  if (_wdupenv_s(&buffer,&length,name) == 0)
  {
    if ((buffer != (wchar_t *) NULL) && (length > 0))
    {
      value=wstring(buffer);
      free(buffer);
      return(value);
    }
  }

  return(value);
}

bool TargetPage::hasVisualStudioFolder(const wchar_t *name)
{
  wstring
    path;

  path=getEnvironmentVariable(L"ProgramW6432") + L"\\Microsoft Visual Studio\\" + name;
  if (PathFileExists(path.c_str()))
    return(true);
  path=getEnvironmentVariable(L"ProgramFiles(x86)") + L"\\Microsoft Visual Studio\\" + name;
  return(PathFileExists(path.c_str()) ? true : false);
}

void TargetPage::setVisualStudioVersion()
{
  if (hasVisualStudioFolder(L"2022"))
    _visualStudioVersion=VisualStudioVersion::VS2022;
  else if (hasVisualStudioFolder(L"2019"))
    _visualStudioVersion=VisualStudioVersion::VS2019;
  else if (hasVisualStudioFolder(L"2017"))
    _visualStudioVersion=VisualStudioVersion::VS2017;
  else
    _visualStudioVersion=VSLATEST;
}

