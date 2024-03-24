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
#ifndef __CommandLineInfo__
#define __CommandLineInfo__

#include "ConfigureWizard.h"
#include "Shared.h"

class CommandLineInfo : public CCommandLineInfo
{
public:
  CommandLineInfo(const ConfigureWizard &wizard);

  CommandLineInfo(const CommandLineInfo& obj);

  CommandLineInfo& operator =(const CommandLineInfo& obj);

  bool enableDpc() const;

  bool excludeAliases() const;

  bool excludeDeprecated() const;

  bool includeIncompatibleLicense() const;

  bool includeOptional() const;

  bool installedSupport() const;

  bool noWizard() const;

  Platform platform() const;

  PolicyConfig policyConfig() const;

  QuantumDepth quantumDepth() const;

  SolutionType solutionType() const;

  bool useHDRI() const;

  bool useOpenCL() const;

  bool useOpenMP() const;

  VisualStudioVersion visualStudioVersion() const;

  bool zeroConfigurationSupport() const;

  virtual void ParseParam(const wchar_t* pszParam, BOOL bFlag, BOOL bLast);

private:
  Platform            _platform;
  bool                _enableDpc;
  bool                _excludeAliases;
  bool                _excludeDeprecated;
  bool                _includeIncompatibleLicense;
  bool                _includeOptional;
  bool                _installedSupport;
  bool                _noWizard;
  PolicyConfig        _policyConfig;
  QuantumDepth        _quantumDepth;
  SolutionType        _solutionType;
  bool                _useHDRI;
  bool                _useOpenCL;
  bool                _useOpenMP;
  VisualStudioVersion _visualStudioVersion;
  bool                _zeroConfigurationSupport;
};

#endif // __CommandLineInfo__
