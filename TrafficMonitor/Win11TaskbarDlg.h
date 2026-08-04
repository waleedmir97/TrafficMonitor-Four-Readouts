#pragma once
#include "TaskBarDlg.h"

class CWin11TaskbarDlg : public CTaskBarDlg
{
private:
    void InitTaskbarWnd() override;
    void AdjustTaskbarWndPos(bool force_adjust) override;
    void ResetTaskbarPos() override;
    void CheckTaskbarOnTopOrBottom() override;

    HWND m_hNotify{};
    HWND m_hStart{};
    CRect m_rcNotify;
    CRect m_rcStart;
    CRect m_last_taskbar_rect;
    CRect m_last_notify_rect;
    CRect m_last_start_rect;
    UINT m_last_layout_dpi{};
};
