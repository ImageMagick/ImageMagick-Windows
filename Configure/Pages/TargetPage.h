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
#ifndef __TargetPage__
#define __TargetPage__

#include "..\Shared.h"

class TargetPage : public CPropertyPage
{
  DECLARE_DYNCREATE(TargetPage)

public:

  TargetPage();

  ~TargetPage();

  bool enableDpc() const;
  void enableDpc(bool value);

  bool excludeAliases() const;
  void excludeAliases(bool value);

  bool excludeDeprecated() const;
  void excludeDeprecated(bool value);

  bool includeIncompatibleLicense() const;
  void includeIncompatibleLicense(bool value);

  bool includeOptional() const;
  void includeOptional(bool value);

  bool installedSupport() const;
  void installedSupport(bool value);

  Platform platform() const;
  void platform(Platform value);

  PolicyConfig policyConfig() const;
  void policyConfig(PolicyConfig value);

  QuantumDepth quantumDepth() const;
  void quantumDepth(QuantumDepth value);

  SolutionType solutionType() const;
  void solutionType(SolutionType value);

  bool useHDRI() const;
  void useHDRI(bool value);

  bool useOpenCL() const;
  void useOpenCL(bool value);

  bool useOpenMP() const;
  void useOpenMP(bool value);

  VisualStudioVersion visualStudioVersion() const;
  void visualStudioVersion(VisualStudioVersion value);

  bool zeroConfigurationSupport() const;
  void zeroConfigurationSupport(bool value);

protected:

  virtual void DoDataExchange(CDataExchange* pDX);

  virtual BOOL OnInitDialog();

  DECLARE_MESSAGE_MAP()

private:

  wstring getEnvironmentVariable(const wchar_t *name);

  bool hasVisualStudioFolder(const wchar_t *name);

  void setVisualStudioVersion();

  Platform            _platform;
  BOOL                _enableDpc;
  BOOL                _excludeAliases;
  BOOL                _excludeDeprecated;
  BOOL                _includeIncompatibleLicense;
  BOOL                _includeOptional;
  BOOL                _installedSupport;
  PolicyConfig        _policyConfig;
  QuantumDepth        _quantumDepth;
  SolutionType        _solutionType;
  BOOL                _useHDRI;
  BOOL                _useOpenCL;
  BOOL                _useOpenMP;
  VisualStudioVersion _visualStudioVersion;
  BOOL                _zeroConfigurationSupport;
};

#endif // __TargetPage__
