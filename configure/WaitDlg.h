//***************************************************************************
//
//  WaitDlg.h
//
//***************************************************************************

#include "resource.h"		// main symbols

class CWaitDialog : public CDialog
{
protected:
    bool* m_pFlag;

public:
    CWaitDialog (bool*, LPCTSTR = NULL, LPCTSTR = NULL, CWnd* pParentWnd = NULL);
    virtual ~CWaitDialog ();

    virtual void OnCancel ();

    BOOL Pump ();
    void SetPercentComplete (int);
    void SetMessageText (LPCTSTR);
    void Close ();
};
