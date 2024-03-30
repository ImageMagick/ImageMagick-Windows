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
#include "resource.h"
#include "ConfigureWizard.h"
#include "CommandLineInfo.h"

IMPLEMENT_DYNAMIC(ConfigureWizard,CPropertySheet)

ConfigureWizard::ConfigureWizard(CWnd* pWndParent)
  : CPropertySheet(IDS_PROPSHT_CAPTION,pWndParent)
{
  AddPage(&_welcomePage);
  AddPage(&_targetPage);
  AddPage(&_finishedPage);

  _isImageMagick7=filesystem::exists(pathFromRoot(L"ImageMagick\\MagickCore"));
  _targetPage.useHDRI(_isImageMagick7);

  SetWizardMode();
}

ConfigureWizard::~ConfigureWizard()
{
}

wstring ConfigureWizard::binDirectory() const
{
  return(L"Output\\bin\\");
}

wstring ConfigureWizard::channelMaskDepth() const
{
  if (!_isImageMagick7)
    return(L"");

  if ((visualStudioVersion() >= VisualStudioVersion::VS2022) && (platform() != Platform::X86))
    return(L"64");
  else
    return(L"32");
}

bool ConfigureWizard::enableDpc() const
{
  return(_targetPage.enableDpc());
}

bool ConfigureWizard::excludeAliases() const
{
  return(_targetPage.excludeAliases());
}

bool ConfigureWizard::excludeDeprecated() const
{
  return(_targetPage.excludeDeprecated());
}

bool ConfigureWizard::includeIncompatibleLicense() const
{
  return(_targetPage.includeIncompatibleLicense());
}

bool ConfigureWizard::includeOptional() const
{
  return(_targetPage.includeOptional());
}

bool ConfigureWizard::installedSupport() const
{
	return(_targetPage.installedSupport());
}

wstring ConfigureWizard::machineName() const
{
  switch (platform())
  {
    case Platform::X86: return(L"X86");
    case Platform::X64: return(L"X64");
    case Platform::ARM64: return(L"ARM64");
    default: throw;
  }
}

Platform ConfigureWizard::platform() const
{
  return(_targetPage.platform());
}

wstring ConfigureWizard::platformName() const
{
  switch (platform())
  {
    case Platform::X86: return(L"Win32");
    case Platform::X64: return(L"x64");
    case Platform::ARM64: return(L"ARM64");
    default: throw;
  }
}

wstring ConfigureWizard::platformAlias() const
{
  switch (platform())
  {
    case Platform::X86: return(L"x86");
    case Platform::X64: return(L"x64");
    case Platform::ARM64: return(L"arm64");
    default: throw;
  }
}

PolicyConfig ConfigureWizard::policyConfig() const
{
  return(_targetPage.policyConfig());
}

QuantumDepth ConfigureWizard::quantumDepth() const
{
  return(_targetPage.quantumDepth());
}

wstring ConfigureWizard::solutionName() const
{
  if (solutionType() == SolutionType::DYNAMIC_MT)
    return(L"Dynamic");
  else if (solutionType() == SolutionType::STATIC_MTD)
    return(L"StaticDLL");
  else if (solutionType() == SolutionType::STATIC_MT)
    return(L"Static");
  else
    return(L"ThisShouldNeverHappen");
}

SolutionType ConfigureWizard::solutionType() const
{
  return(_targetPage.solutionType());
}

bool ConfigureWizard::useHDRI() const
{
  return(_targetPage.useHDRI());
}

bool ConfigureWizard::useOpenCL() const
{
  return(_targetPage.useOpenCL());
}

bool ConfigureWizard::useOpenMP() const
{
  return(_targetPage.useOpenMP());
}

VisualStudioVersion ConfigureWizard::visualStudioVersion() const
{
  return(_targetPage.visualStudioVersion());
}

wstring ConfigureWizard::visualStudioVersionName() const
{
  switch(_targetPage.visualStudioVersion())
  {
    case VisualStudioVersion::VS2017: return(L"VS2017");
    case VisualStudioVersion::VS2019: return(L"VS2019");
    case VisualStudioVersion::VS2022: return(L"VS2022");
    default: return(L"VS");
  }
}

bool ConfigureWizard::zeroConfigurationSupport() const
{
  return(_targetPage.zeroConfigurationSupport());
}

void ConfigureWizard::parseCommandLineInfo(const CommandLineInfo &info)
{
  _targetPage.platform(info.platform());
  _targetPage.enableDpc(info.enableDpc());
  _targetPage.excludeAliases(info.excludeAliases());
  _targetPage.excludeDeprecated(info.excludeDeprecated());
  _targetPage.includeIncompatibleLicense(info.includeIncompatibleLicense());
  _targetPage.includeOptional(info.includeOptional());
  _targetPage.installedSupport(info.installedSupport());
  _targetPage.policyConfig(info.policyConfig());
  _targetPage.quantumDepth(info.quantumDepth());
  _targetPage.solutionType(info.solutionType());
  _targetPage.useHDRI(info.useHDRI());
  _targetPage.useOpenCL(info.useOpenCL());
  _targetPage.useOpenMP(info.useOpenMP());
  _targetPage.visualStudioVersion(info.visualStudioVersion());
  _targetPage.zeroConfigurationSupport(info.zeroConfigurationSupport());
}

BEGIN_MESSAGE_MAP(ConfigureWizard,CPropertySheet)
END_MESSAGE_MAP()
