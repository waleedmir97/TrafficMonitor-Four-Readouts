#pragma once
#include "BaseDialog.h"

class CAppAlreadyRuningDlg : public CBaseDialog
{
    DECLARE_DYNAMIC(CAppAlreadyRuningDlg)

public:
    CAppAlreadyRuningDlg(HWND handel, CWnd* pParent = nullptr);
    virtual ~CAppAlreadyRuningDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_APP_ALREAD_RUNING_DIALOG };
#endif

private:
    HWND m_handle{};

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual CString GetDialogName() const override;

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedExitInstButton();
    afx_msg void OnBnClickedOpenSettingsButton();
    afx_msg void OnBnClickedRestartTaskbarButton();
};