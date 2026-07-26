#include "stdafx.h"
#include "Win11TaskbarDlg.h"
#include "WindowsSettingHelper.h"
#include <UIAutomation.h>

#pragma comment(lib, "UIAutomationCore.lib")

namespace
{
    constexpr int WIDGETS_GAP = 6;

    bool IsSameRect(const CRect& left, const CRect& right)
    {
        return left.left == right.left && left.top == right.top &&
            left.right == right.right && left.bottom == right.bottom;
    }
}

void CWin11TaskbarDlg::AdjustTaskbarWndPos(bool force_adjust)
{
    m_rcNotify.SetRectEmpty();
    if (::IsWindow(m_hNotify))
    {
        ::GetWindowRect(m_hNotify, m_rcNotify);
        m_rcNotify.MoveToXY(m_rcNotify.left - m_rcTaskbar.left, m_rcNotify.top - m_rcTaskbar.top);
    }

    m_rcStart.SetRectEmpty();
    if (::IsWindow(m_hStart))
    {
        ::GetWindowRect(m_hStart, m_rcStart);
        m_rcStart.MoveToXY(m_rcStart.left - m_rcTaskbar.left, m_rcStart.top - m_rcTaskbar.top);
    }

    CRect widgets_rect;
    GetWidgetsButtonRect(widgets_rect);
    const bool widgets_rect_changed = !IsSameRect(widgets_rect, m_rcWidgets);
    m_rcWidgets = widgets_rect;

    m_rect.right = m_rect.left + m_window_width;
    m_rect.bottom = m_rect.top + m_window_height;
    const bool taskbar_rect_changed = m_rcTaskbar.left != m_last_taskbar_rect.left ||
        m_rcTaskbar.top != m_last_taskbar_rect.top || m_rcTaskbar.right != m_last_taskbar_rect.right ||
        m_rcTaskbar.bottom != m_last_taskbar_rect.bottom;
    if (force_adjust || taskbar_rect_changed || m_rcNotify.Width() != m_last_notify_width ||
        m_rcStart.left != m_last_start_pos || widgets_rect_changed)
    {
        m_last_taskbar_rect = m_rcTaskbar;
        m_last_notify_width = m_rcNotify.Width();
        m_last_start_pos = m_rcStart.left;

        bool placed_after_widgets = false;
        if (!m_rcWidgets.IsRectEmpty())
        {
            const int target_x = m_rcWidgets.right + DPI(WIDGETS_GAP);
            int available_right = m_rcTaskbar.Width();
            if (!m_rcStart.IsRectEmpty() && m_rcStart.left > m_rcWidgets.right)
                available_right = m_rcStart.left - DPI(WIDGETS_GAP);

            if (target_x + m_rect.Width() <= available_right)
            {
                m_rect.MoveToX(target_x);
                placed_after_widgets = true;
            }
        }

        if (!placed_after_widgets && (!theApp.m_taskbar_data.tbar_wnd_on_left || !CWindowsSettingHelper::IsTaskbarCenterAlign()))
        {
            int notify_x_pos = m_rcNotify.IsRectEmpty() ? 0 : m_rcNotify.left;
            if (notify_x_pos <= 0)
            {
                const int reserved_width = m_is_secondary_display ? DPI(88) : DPI(max(88, theApp.m_taskbar_data.taskbar_right_space_win11));
                notify_x_pos = max(0, m_rcTaskbar.Width() - reserved_width);
            }
            m_rect.MoveToX(notify_x_pos - m_rect.Width() + 2);
        }
        else if (!placed_after_widgets && theApp.m_taskbar_data.tbar_wnd_snap && !m_rcStart.IsRectEmpty())
        {
            m_rect.MoveToX(m_rcStart.left - m_rect.Width() - 2);
        }
        else if (!placed_after_widgets)
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
    m_rcWidgets.SetRectEmpty();
}

void CWin11TaskbarDlg::ResetTaskbarPos()
{
    m_last_taskbar_rect.SetRectEmpty();
    m_last_notify_width = 0;
    m_last_start_pos = 0;
    m_rcWidgets.SetRectEmpty();
}

bool CWin11TaskbarDlg::GetWidgetsButtonRect(CRect& rect)
{
    rect.SetRectEmpty();
    if (!::IsWindow(m_hTaskbar))
        return false;

    const HRESULT init_result = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    IUIAutomation* automation{};
    const HRESULT create_result = ::CoCreateInstance(
        CLSID_CUIAutomation, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&automation));
    if (FAILED(create_result) || automation == nullptr)
    {
        if (SUCCEEDED(init_result))
            ::CoUninitialize();
        return false;
    }

    bool found = false;
    IUIAutomationElement* taskbar_element{};
    if (SUCCEEDED(automation->ElementFromHandle(m_hTaskbar, &taskbar_element)) && taskbar_element != nullptr)
    {
        VARIANT automation_id;
        ::VariantInit(&automation_id);
        automation_id.vt = VT_BSTR;
        automation_id.bstrVal = ::SysAllocString(L"WidgetsButton");

        if (automation_id.bstrVal != nullptr)
        {
            IUIAutomationCondition* condition{};
            if (SUCCEEDED(automation->CreatePropertyCondition(
                    UIA_AutomationIdPropertyId, automation_id, &condition)) && condition != nullptr)
            {
                IUIAutomationElement* widgets_button{};
                if (SUCCEEDED(taskbar_element->FindFirst(TreeScope_Descendants, condition, &widgets_button)) &&
                    widgets_button != nullptr)
                {
                    RECT widgets_rect{};
                    if (SUCCEEDED(widgets_button->get_CurrentBoundingRectangle(&widgets_rect)) &&
                        widgets_rect.right > widgets_rect.left && widgets_rect.bottom > widgets_rect.top)
                    {
                        rect = widgets_rect;
                        rect.OffsetRect(-m_rcTaskbar.left, -m_rcTaskbar.top);
                        found = true;
                    }
                    widgets_button->Release();
                }
                condition->Release();
            }
            ::VariantClear(&automation_id);
        }
        taskbar_element->Release();
    }

    automation->Release();
    if (SUCCEEDED(init_result))
        ::CoUninitialize();
    return found;
}

HWND CWin11TaskbarDlg::GetParentHwnd()
{
    return m_hTaskbar;
}

void CWin11TaskbarDlg::CheckTaskbarOnTopOrBottom()
{
    m_taskbar_on_top_or_bottom = true;
}
