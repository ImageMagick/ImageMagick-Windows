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
#include "CommandLineInfo.h"
#include "ConfigureApp.h"
#include "ConfigureWizard.h"
#include "Solution.h"
#include "WaitDialog.h"

BEGIN_MESSAGE_MAP(ConfigureApp, CWinApp)
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

ConfigureApp theApp;

ConfigureApp::ConfigureApp()
{
}

BOOL ConfigureApp::Init()
{
  ConfigureWizard
    wizard;
  
  INT_PTR
    response;

  Solution
    solution(wizard);

  WaitDialog
    waitDialog;

  CommandLineInfo info=CommandLineInfo(wizard);
  ParseCommandLine(info);

  wizard.parseCommandLineInfo(info);

  solution.loadProjects();

  response=ID_WIZFINISH;
  if (info.noWizard() == FALSE)
    response=wizard.DoModal();

  if (response != ID_WIZFINISH)
    return(FALSE);

  solution.write(waitDialog);
  return(TRUE);
}


BOOL ConfigureApp::InitInstance()
{
  if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
      try
      {
        return Init();
      }
      catch (exception ex)
      {
        FILE
          *fpstderr=stderr;

        freopen_s(&fpstderr, "CONOUT$", "w", stderr);
        cerr << ex.what() << endl;
        return(FALSE);
      }
    }
  else
    return Init();
}
