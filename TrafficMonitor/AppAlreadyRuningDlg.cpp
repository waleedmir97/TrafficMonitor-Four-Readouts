// AppAlreadyRuningDlg.cpp: implementation file
//

#include "stdafx.h"
#include "TrafficMonitor.h"
#include "AppAlreadyRuningDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CAppAlreadyRuningDlg, CBaseDialog)

CAppAlreadyRuningDlg::CAppAlreadyRuningDlg(HWND handel, CWnd* pParent /*=nullptr*/)
    : CBaseDialog(IDD_APP_ALREAD_RUNING_DIALOG, pParent), m_handle(handel)
{
}

CAppAlreadyRuningDlg::~CAppAlreadyRuningDlg()
{
}

void CAppAlreadyRuningDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
}

CString CAppAlreadyRuningDlg::GetDialogName() const
{
    return _T("AppAlreadyRuningDlg");
}

BEGIN_MESSAGE_MAP(CAppAlreadyRuningDlg, CBaseDialog)
    ON_BN_CLICKED(IDC_EXIT_INST_BUTTON, &CAppAlreadyRuningDlg::OnBnClickedExitInstButton)
    ON_BN_CLICKED(IDC_OPEN_SETTINGS_BUTTON, &CAppAlreadyRuningDlg::OnBnClickedOpenSettingsButton)
    ON_BN_CLICKED(IDC_SHOW_HIDE_TASKBAR_WINDOW_BUTTON, &CAppAlreadyRuningDlg::OnBnClickedRestartTaskbarButton)
END_MESSAGE_MAP()

BOOL CAppAlreadyRuningDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
    SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);
    return TRUE;
}

void CAppAlreadyRuningDlg::OnBnClickedExitInstButton()
{
    ::PostMessage(m_handle, WM_COMMAND, ID_APP_EXIT, 0);
    EndDialog(IDOK);
}

void CAppAlreadyRuningDlg::OnBnClickedOpenSettingsButton()
{
    ::PostMessage(m_handle, WM_COMMAND, ID_OPTIONS, 0);
    EndDialog(IDOK);
}

void CAppAlreadyRuningDlg::OnBnClickedRestartTaskbarButton()
{
    ::PostMessage(m_handle, WM_COMMAND, ID_SHOW_TASK_BAR_WND, 0);
    EndDialog(IDOK);
}