#pragma once
#include "BaseDialog.h"

#define OPTION_DLG_NAME _T("OptionsDlg")

class COptionsDlg : public CBaseDialog
{
    DECLARE_DYNAMIC(COptionsDlg)

public:
    COptionsDlg(int tab = 0, CWnd* pParent = NULL);
    virtual ~COptionsDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_OPTIONS_DIALOG };
#endif

protected:
    virtual CString GetDialogName() const override;
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnBnClickedRestartTaskbarButton();
};