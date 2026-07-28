#pragma once
#include "TaskBarDlg.h"
class CClassicalTaskbarDlg :
    public CTaskBarDlg
{
public:

private:
    // 通过 CTaskBarDlg 继承
    virtual void AdjustTaskbarWndPos(bool force_adjust) override;
    void InitTaskbarWnd() override;
    void ResetTaskbarPos() override;

private:
    HWND m_hBar{};      //任务栏窗口二级容器的句柄（只读定位）
    HWND m_hMin{};      //最小化窗口的句柄（只读定位）
    CRect m_rcBar;      //任务栏窗口二级容器的区域
    CRect m_rcMin;      //最小化窗口的区域

    // 通过 CTaskBarDlg 继承
    void CheckTaskbarOnTopOrBottom() override;
};

