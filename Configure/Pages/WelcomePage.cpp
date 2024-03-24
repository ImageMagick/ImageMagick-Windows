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
#include "..\resource.h"
#include "WelcomePage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(WelcomePage, CPropertyPage)

WelcomePage::WelcomePage() : CPropertyPage(IDD_WELCOME_PAGE)
{
}

WelcomePage::~WelcomePage()
{
}

void WelcomePage::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Text(pDX,IDC_WELCOME_TEXT,_welcomeText);
}

BEGIN_MESSAGE_MAP(WelcomePage, CPropertyPage)
END_MESSAGE_MAP()

BOOL WelcomePage::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  _welcomeText  = "Welcome!\n\n";

  _welcomeText += "This is the VisualMagick configuration wizard. It will help you set up ";
  _welcomeText += "your build environment and customize the build process for your needs.";
  _welcomeText += "Its purpose is to create Visual Studio project and solution files.\n\n";

  _welcomeText += "When the build environment has been created you can build the system in ";
  _welcomeText += "the Visual Studio IDE.\n\n";

  _welcomeText += "Good Luck - and thanks for all the fish!";

  UpdateData(FALSE);

  return TRUE;
}

BOOL WelcomePage::OnSetActive() 
{
  CPropertySheet* psheet = (CPropertySheet*) GetParent();
  psheet->SetWizardButtons(PSWIZB_NEXT);

  return CPropertyPage::OnSetActive();
}
