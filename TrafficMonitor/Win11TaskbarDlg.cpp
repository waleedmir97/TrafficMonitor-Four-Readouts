#include "stdafx.h"
#include "Win11TaskbarDlg.h"

namespace
{
    constexpr int TASKBAR_ITEM_GAP = 6;

    bool RectsEqual(const CRect& left, const CRect& right)
    {
        return left.left == right.left && left.top == right.top &&
            left.right == right.right && left.bottom == right.bottom;
    }
}

void CWin11TaskbarDlg::AdjustTaskbarWndPos(bool force_adjust)
{
    if (!::IsWindow(m_hNotify))
        m_hNotify = ::FindWindowEx(m_hTaskbar, nullptr, L"TrayNotifyWnd", nullptr);
    if (!::IsWindow(m_hStart))
        m_hStart = ::FindWindowEx(m_hTaskbar, nullptr, L"Start", nullptr);

    m_rcNotify.SetRectEmpty();
    if (::IsWindow(m_hNotify) && ::GetWindowRect(m_hNotify, m_rcNotify))
        m_rcNotify.OffsetRect(-m_rcTaskbar.left, -m_rcTaskbar.top);

    m_rcStart.SetRectEmpty();
    if (::IsWindow(m_hStart) && ::GetWindowRect(m_hStart, m_rcStart))
        m_rcStart.OffsetRect(-m_rcTaskbar.left, -m_rcTaskbar.top);

    m_rect.right = m_rect.left + m_window_width;
    m_rect.bottom = m_rect.top + m_window_height;

    const bool taskbar_rect_changed = !RectsEqual(m_rcTaskbar, m_last_taskbar_rect);
    const bool notify_rect_changed = !RectsEqual(m_rcNotify, m_last_notify_rect);
    const bool start_rect_changed = !RectsEqual(m_rcStart, m_last_start_rect);
    const bool dpi_changed = m_last_layout_dpi != m_taskbar_dpi;
    if (!force_adjust && !taskbar_rect_changed && !notify_rect_changed &&
        !start_rect_changed && !dpi_changed)
    {
        return;
    }

    m_last_taskbar_rect = m_rcTaskbar;
    m_last_notify_rect = m_rcNotify;
    m_last_start_rect = m_rcStart;
    m_last_layout_dpi = m_taskbar_dpi;

    const int gap = DPI(TASKBAR_ITEM_GAP);
    const bool horizontal = m_rcTaskbar.Width() >= m_rcTaskbar.Height();
    if (horizontal)
    {
        int x = gap;
        const int notify_left = m_rcNotify.IsRectEmpty()
            ? m_rcTaskbar.Width()
            : m_rcNotify.left;
        const int start_left = m_rcStart.IsRectEmpty()
            ? notify_left
            : m_rcStart.left;

        if (theApp.m_taskbar_data.tbar_wnd_on_left)
        {
            // Windows exposes the Widgets/Weather button through XAML rather
            // than a stable child HWND. Use the configured reserved width as
            // its edge, then stay clear of the centered Start/task-button band.
            const int widgets_edge = theApp.m_taskbar_data.avoid_overlap_with_widgets
                ? DPI(max(0, theApp.m_taskbar_data.taskbar_left_space_win11))
                : gap;
            const int available_right = max(0, start_left - gap);
            if (widgets_edge + m_rect.Width() <= available_right)
                x = widgets_edge;
            else if (!m_rcStart.IsRectEmpty())
                x = m_rcStart.left - m_rect.Width() - gap;
        }
        else
        {
            x = notify_left - m_rect.Width() - gap;
        }

        x += DPI(theApp.m_taskbar_data.window_offset_left);
        x = max(0, min(x, max(0, m_rcTaskbar.Width() - m_rect.Width())));

        int y = (m_rcTaskbar.Height() - m_rect.Height()) / 2;
        if (!m_rcStart.IsRectEmpty())
            y = m_rcStart.top + (m_rcStart.Height() - m_rect.Height()) / 2;
        y += DPI(theApp.m_taskbar_data.window_offset_top);
        y = max(0, min(y, max(0, m_rcTaskbar.Height() - m_rect.Height())));
        m_rect.MoveToXY(x, y);
    }
    else
    {
        int x = (m_rcTaskbar.Width() - m_rect.Width()) / 2;
        int y = (m_rcNotify.IsRectEmpty() ? m_rcTaskbar.Height() : m_rcNotify.top) -
            m_rect.Height() - gap;
        x += DPI(theApp.m_taskbar_data.window_offset_left);
        y += DPI(theApp.m_taskbar_data.window_offset_top);
        x = max(0, min(x, max(0, m_rcTaskbar.Width() - m_rect.Width())));
        y = max(0, min(y, max(0, m_rcTaskbar.Height() - m_rect.Height())));
        m_rect.MoveToXY(x, y);
    }

    MoveWindow(m_rect);
}

void CWin11TaskbarDlg::InitTaskbarWnd()
{
    m_hNotify = ::FindWindowEx(m_hTaskbar, nullptr, L"TrayNotifyWnd", nullptr);
    m_hStart = ::FindWindowEx(m_hTaskbar, nullptr, L"Start", nullptr);
    ResetTaskbarPos();
}

void CWin11TaskbarDlg::ResetTaskbarPos()
{
    m_last_taskbar_rect.SetRectEmpty();
    m_last_notify_rect.SetRectEmpty();
    m_last_start_rect.SetRectEmpty();
    m_last_layout_dpi = 0;
}

void CWin11TaskbarDlg::CheckTaskbarOnTopOrBottom()
{
    m_taskbar_on_top_or_bottom =
        m_rcTaskbar.Width() >= m_rcTaskbar.Height();
}
