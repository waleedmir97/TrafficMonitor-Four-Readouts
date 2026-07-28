#include "stdafx.h"
#include "Win11TaskbarDlg.h"
#include "WindowsSettingHelper.h"
void CWin11TaskbarDlg::AdjustTaskbarWndPos(bool force_adjust)
{
    if (!::IsWindow(m_hNotify))
        m_hNotify = ::FindWindowEx(m_hTaskbar, nullptr, L"TrayNotifyWnd", nullptr);
    m_rcNotify.SetRectEmpty();
    if (::IsWindow(m_hNotify))
    {
        ::GetWindowRect(m_hNotify, m_rcNotify);
        m_rcNotify.MoveToXY(m_rcNotify.left - m_rcTaskbar.left, m_rcNotify.top - m_rcTaskbar.top);
    }

    if (!::IsWindow(m_hStart))
        m_hStart = ::FindWindowEx(m_hTaskbar, nullptr, L"Start", nullptr);
    m_rcStart.SetRectEmpty();
    if (::IsWindow(m_hStart))
    {
        ::GetWindowRect(m_hStart, m_rcStart);
        m_rcStart.MoveToXY(m_rcStart.left - m_rcTaskbar.left, m_rcStart.top - m_rcTaskbar.top);
    }

    m_rect.right = m_rect.left + m_window_width;
    m_rect.bottom = m_rect.top + m_window_height;
    const bool taskbar_rect_changed = m_rcTaskbar.left != m_last_taskbar_rect.left ||
        m_rcTaskbar.top != m_last_taskbar_rect.top || m_rcTaskbar.right != m_last_taskbar_rect.right ||
        m_rcTaskbar.bottom != m_last_taskbar_rect.bottom;
    const bool notify_rect_changed = m_rcNotify.left != m_last_notify_rect.left ||
        m_rcNotify.top != m_last_notify_rect.top || m_rcNotify.right != m_last_notify_rect.right ||
        m_rcNotify.bottom != m_last_notify_rect.bottom;
    const bool start_rect_changed = m_rcStart.left != m_last_start_rect.left ||
        m_rcStart.top != m_last_start_rect.top || m_rcStart.right != m_last_start_rect.right ||
        m_rcStart.bottom != m_last_start_rect.bottom;
    if (force_adjust || taskbar_rect_changed || notify_rect_changed || start_rect_changed)
    {
        m_last_taskbar_rect = m_rcTaskbar;
        m_last_notify_rect = m_rcNotify;
        m_last_start_rect = m_rcStart;

        if (!theApp.m_taskbar_data.tbar_wnd_on_left || !CWindowsSettingHelper::IsTaskbarCenterAlign())
        {
            int notify_x_pos = m_rcNotify.IsRectEmpty() ? 0 : m_rcNotify.left;
            if (notify_x_pos <= 0)
            {
                const int reserved_width = m_is_secondary_display ? DPI(88) : DPI(max(88, theApp.m_taskbar_data.taskbar_right_space_win11));
                notify_x_pos = max(0, m_rcTaskbar.Width() - reserved_width);
            }
            m_rect.MoveToX(notify_x_pos - m_rect.Width() + 2);
        }
        else if (theApp.m_taskbar_data.tbar_wnd_snap && !m_rcStart.IsRectEmpty())
        {
            m_rect.MoveToX(m_rcStart.left - m_rect.Width() - 2);
        }
        else
        {
            m_rect.MoveToX(2);
        }

        int x = m_rect.left + DPI(theApp.m_taskbar_data.window_offset_left);
        const int max_x = max(0, m_rcTaskbar.Width() - m_rect.Width());
        x = max(0, min(x, max_x));
        m_rect.MoveToX(x);

        const int anchor_height = m_rcStart.Height() > 0 ? m_rcStart.Height() : m_rcTaskbar.Height();
        int y = (anchor_height - m_rect.Height()) / 2 + (m_rcTaskbar.Height() - anchor_height) + DPI(theApp.m_taskbar_data.window_offset_top);
        const int max_y = max(0, m_rcTaskbar.Height() - m_rect.Height());
        y = max(0, min(y, max_y));
        m_rect.MoveToY(y);

        MoveWindow(m_rect);
    }
}

void CWin11TaskbarDlg::InitTaskbarWnd()
{
    m_hNotify = ::FindWindowEx(m_hTaskbar, nullptr, L"TrayNotifyWnd", nullptr);
    m_hStart = ::FindWindowEx(m_hTaskbar, nullptr, L"Start", nullptr);
    m_last_taskbar_rect.SetRectEmpty();
    m_last_notify_rect.SetRectEmpty();
    m_last_start_rect.SetRectEmpty();
}

void CWin11TaskbarDlg::ResetTaskbarPos()
{
    m_last_taskbar_rect.SetRectEmpty();
    m_last_notify_rect.SetRectEmpty();
    m_last_start_rect.SetRectEmpty();
}

void CWin11TaskbarDlg::CheckTaskbarOnTopOrBottom()
{
    m_taskbar_on_top_or_bottom = true;
}
