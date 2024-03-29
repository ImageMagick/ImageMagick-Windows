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
#ifndef __WelcomePage__
#define __WelcomePage__

class WelcomePage : public CPropertyPage
{
  DECLARE_DYNCREATE(WelcomePage)

public:

  WelcomePage();

  ~WelcomePage();

  virtual BOOL OnSetActive();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);

  virtual BOOL OnInitDialog();

private:

  CString _welcomeText;

  DECLARE_MESSAGE_MAP()
};

#endif // __WelcomePage__
