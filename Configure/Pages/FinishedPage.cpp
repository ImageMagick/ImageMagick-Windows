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
#include "FinishedPage.h"

IMPLEMENT_DYNCREATE(FinishedPage,CPropertyPage)

FinishedPage::FinishedPage() : CPropertyPage(IDD_FINISHED_PAGE)
{
  _text="";
}

FinishedPage::~FinishedPage()
{
}

BOOL FinishedPage::OnSetActive() 
{
  CPropertySheet
     *propertySheet;

  propertySheet=(CPropertySheet*) GetParent();
  propertySheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);

  return CPropertyPage::OnSetActive();
}

void FinishedPage::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);

  DDX_Text(pDX,IDC_FINISHED_TEXT,_text);
}

BOOL FinishedPage::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  _text  = "Congratulations!\n\n";

  _text += "You have finished the ImageMagick configuration. Now press finish to finally";
  _text += " produce the Visual Studio compatible files.\n\n";
  _text += "The dynamic (DLL) build is named ImageMagickDynamic while the static build";
  _text += " configurations are named ImageMagickStatic and ImageMagickStaticDLL.\n\n";
  _text += "Good luck!";

  UpdateData(FALSE);

  return(TRUE);
}

BEGIN_MESSAGE_MAP(FinishedPage, CPropertyPage)
END_MESSAGE_MAP()
