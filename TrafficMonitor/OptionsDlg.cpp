// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TrafficMonitor.h"
#include "OptionsDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(COptionsDlg, CBaseDialog)

COptionsDlg::COptionsDlg(int, CWnd* pParent /*=NULL*/)
    : CBaseDialog(IDD_OPTIONS_DIALOG, pParent)
{
}

COptionsDlg::~COptionsDlg()
{
}

CString COptionsDlg::GetDialogName() const
{
    return OPTION_DLG_NAME;
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COptionsDlg, CBaseDialog)
    ON_BN_CLICKED(IDC_SHOW_HIDE_TASKBAR_WINDOW_BUTTON, &COptionsDlg::OnBnClickedRestartTaskbarButton)
END_MESSAGE_MAP()

BOOL COptionsDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
    SetIcon(theApp.GetMenuIcon(IDI_SETTINGS), FALSE);
    CheckDlgButton(IDC_AUTO_RUN_CHECK, theApp.IsAutoStartEnabled() ? BST_CHECKED : BST_UNCHECKED);
    return TRUE;
}

void COptionsDlg::OnOK()
{
    const bool auto_start_enabled = (IsDlgButtonChecked(IDC_AUTO_RUN_CHECK) == BST_CHECKED);
    const bool auto_run_registered = theApp.GetAutoRun(nullptr);
    const bool update_needed = (auto_start_enabled != theApp.IsAutoStartEnabled())
        || (auto_start_enabled && !auto_run_registered)
        || (!auto_start_enabled && auto_run_registered);

    if (update_needed && !theApp.SetAutoStartEnabled(auto_start_enabled))
    {
        AfxMessageBox(_T("Unable to update automatic startup."), MB_OK | MB_ICONWARNING);
        return;
    }

    CBaseDialog::OnOK();
}

void COptionsDlg::OnBnClickedRestartTaskbarButton()
{
    if (theApp.m_pMainWnd != nullptr)
        ::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_COMMAND, ID_SHOW_TASK_BAR_WND, 0);
}