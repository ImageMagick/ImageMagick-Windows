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
#include "WaitDialog.h"

WaitDialog::WaitDialog() : CDialog ()
{
  Create(IDD_WAITDIALOG);
}

WaitDialog::~WaitDialog()
{
  if (!IsWindow(m_hWnd))
    return;

  DestroyWindow();
}

int WaitDialog::getSteps() const
{
  return(_steps);
}

void WaitDialog::setSteps(const int steps)
{
  ShowWindow(SW_SHOW);
  _steps=steps;
  _current=0;
}

void WaitDialog::nextStep(const wstring &description)
{
  setPercentComplete((++_current * 100)/_steps);
  setMessageText(description);
  pump();
}

void WaitDialog::pump()
{
  LONG
    idle;

  MSG
    msg;

  while (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
  {
    if (!AfxGetApp()->PumpMessage())
      PostQuitMessage(0);
  }

  idle=0;
  while(AfxGetApp()->OnIdle(idle++));
}

void WaitDialog::setMessageText(const wstring &text)
{
  CStatic
    *control;

  if (!IsWindow(m_hWnd))
    return;

  control=(CStatic *) GetDlgItem(IDC_MSGCTRL);
  control->SetWindowText(text.c_str());
}

void WaitDialog::setPercentComplete(int percent)
{
  CProgressCtrl
    *control;

  if (!IsWindow(m_hWnd))
    return;

  if (percent < 0)
    percent=0;
  else if (percent > 100)
    percent=100;

  control=(CProgressCtrl *) GetDlgItem(IDC_PROGRESSCTRL);
  control->SetPos(percent);
}
