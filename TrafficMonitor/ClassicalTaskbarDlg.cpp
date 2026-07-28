#include "stdafx.h"
#include "ClassicalTaskbarDlg.h"

void CClassicalTaskbarDlg::AdjustTaskbarWndPos(bool force_adjust)
{
    // Explorer can recreate these children after a shell restart. Reacquire
    // them opportunistically, but use them only as read-only geometry hints.
    if (!::IsWindow(m_hBar) || !::IsWindow(m_hMin))
        InitTaskbarWnd();

    CRect anchor_rect = m_rcTaskbar;
    if (::IsWindow(m_hMin) && ::GetWindowRect(m_hMin, m_rcMin) && !m_rcMin.IsRectEmpty())
        anchor_rect = m_rcMin;

    CRect target_rect(0, 0, m_window_width, m_window_height);
    const int inset = DPI(2);
    if (m_taskbar_on_top_or_bottom)
    {
        int x = theApp.m_taskbar_data.tbar_wnd_on_left
            ? anchor_rect.left
            : anchor_rect.right - target_rect.Width();
        const int min_x = m_rcTaskbar.left + inset;
        int max_x = m_rcTaskbar.right - target_rect.Width() - inset;
        if (max_x < min_x)
            max_x = min_x;
        x = max(min_x, min(x, max_x));

        int y = m_rcTaskbar.top + (m_rcTaskbar.Height() - target_rect.Height()) / 2;
        if (theApp.m_taskbar_data.horizontal_arrange && theApp.m_win_version.IsWindows7())
            y += DPI(1);
        target_rect.MoveToXY(x - m_rcTaskbar.left, y - m_rcTaskbar.top);
    }
    else
    {
        int y = theApp.m_taskbar_data.tbar_wnd_on_left
            ? anchor_rect.top
            : anchor_rect.bottom - target_rect.Height();
        const int min_y = m_rcTaskbar.top + inset;
        int max_y = m_rcTaskbar.bottom - target_rect.Height() - inset;
        if (max_y < min_y)
            max_y = min_y;
        y = max(min_y, min(y, max_y));

        const int x = m_rcTaskbar.left + (m_rcTaskbar.Width() - target_rect.Width()) / 2;
        target_rect.MoveToXY(x - m_rcTaskbar.left, y - m_rcTaskbar.top);
    }

    const bool position_changed = target_rect.left != m_rect.left || target_rect.top != m_rect.top ||
        target_rect.right != m_rect.right || target_rect.bottom != m_rect.bottom;
    if (force_adjust || position_changed)
    {
        m_rect = target_rect;
        MoveWindow(m_rect);
    }
}

void CClassicalTaskbarDlg::InitTaskbarWnd()
{
    m_hBar = ::FindWindowEx(m_hTaskbar, 0, L"ReBarWindow32", NULL); //寻找二级容器的句柄
    if (m_hBar == NULL)
        m_hBar = ::FindWindowEx(m_hTaskbar, nullptr, L"WorkerW", NULL);
    m_hMin = ::FindWindowEx(m_hBar, 0, L"MSTaskSwWClass", NULL);    //寻找最小化窗口的句柄
    if (m_hMin == NULL)
        m_hMin = ::FindWindowEx(m_hBar, 0, L"MSTaskListWClass", NULL);    //寻找最小化窗口的句柄

    m_rcMin.SetRectEmpty();
    m_rcBar.SetRectEmpty();
    if (::IsWindow(m_hMin))
        ::GetWindowRect(m_hMin, m_rcMin);
    if (::IsWindow(m_hBar))
        ::GetWindowRect(m_hBar, m_rcBar);
}

void CClassicalTaskbarDlg::ResetTaskbarPos()
{
    // The overlay never changes Explorer-owned window geometry, so there is
    // nothing in the taskbar process to restore.
    m_rcMin.SetRectEmpty();
    m_rcBar.SetRectEmpty();
}

void CClassicalTaskbarDlg::CheckTaskbarOnTopOrBottom()
{
    CRect rect;
    CRect rcMin;
    CRect rcBar;
    if (m_hTaskbar != 0)
    {
        //::GetWindowRect(m_hMin, rcMin); //获得最小化窗口的区域
        //::GetWindowRect(m_hBar, rcBar); //获得二级容器的区域
        //if (m_left_space == 0)
        //    m_left_space = rcMin.left - rcBar.left;
        //if (m_top_space == 0)
        //    m_top_space = rcMin.top - rcBar.top;

        ::GetWindowRect(m_hTaskbar, rect);          //获取任务栏的矩形区域
        m_taskbar_on_top_or_bottom = (rect.Width() >= rect.Height());     //如果任务栏的宽度大于高度，则任务在屏幕的顶部或底部
    }
    else
    {
        m_taskbar_on_top_or_bottom = true;
    }
}

