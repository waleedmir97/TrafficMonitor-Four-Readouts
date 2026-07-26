#include "stdafx.h"
#include "TaskbarHelper.h"
#include <algorithm>

// 存储显示器信息
struct MonitorInfo
{
    HMONITOR hMonitor;
    CRect rect;
};

// 存储任务栏信息
struct TaskbarInfo
{
    HWND hwnd;
    CRect rect;
};

std::vector<MonitorInfo> monitors;
std::vector<TaskbarInfo> taskbars;

// 枚举显示器的回调函数
static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    MonitorInfo info;
    info.hMonitor = hMonitor;
    info.rect = *lprcMonitor;
    if (!info.rect.IsRectEmpty())
        monitors.push_back(info);
    else
        ASSERT(FALSE);
    return TRUE;
}

// 枚举窗口的回调函数
static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    TCHAR className[256];
    GetClassName(hwnd, className, 256);

    // 检查是否是副显示器的任务栏
    if (_tcscmp(className, _T("Shell_SecondaryTrayWnd")) == 0)
    {
        TaskbarInfo info;
        info.hwnd = hwnd;
        GetWindowRect(hwnd, &info.rect);
        taskbars.push_back(info);
    }

    return TRUE;
}

static bool GetMonitorRectForTaskbar(HWND hwnd, CRect& rect)
{
    MONITORINFO monitor_info{ sizeof(monitor_info) };
    HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    if (monitor == nullptr || !::GetMonitorInfo(monitor, &monitor_info))
        return false;
    rect = monitor_info.rcMonitor;
    return true;
}

// Sort secondary taskbars by the monitor Windows actually associates with each host.
static bool CompareTaskbarByMonitorOrder(const TaskbarInfo& a, const TaskbarInfo& b)
{
    CRect a_monitor;
    CRect b_monitor;
    const bool a_has_monitor = GetMonitorRectForTaskbar(a.hwnd, a_monitor);
    const bool b_has_monitor = GetMonitorRectForTaskbar(b.hwnd, b_monitor);
    if (a_has_monitor != b_has_monitor)
        return a_has_monitor;
    if (a_has_monitor && b_has_monitor)
    {
        if (a_monitor.top != b_monitor.top)
            return a_monitor.top < b_monitor.top;
        if (a_monitor.left != b_monitor.left)
            return a_monitor.left < b_monitor.left;
    }
    if (a.rect.top != b.rect.top)
        return a.rect.top < b.rect.top;
    return a.rect.left < b.rect.left;
}

void CTaskbarHelper::GetAllSecondaryDisplayTaskbar(std::vector<HWND>& secondary_taskbars)
{
    monitors.clear();
    taskbars.clear();
    secondary_taskbars.clear();

    // 获取所有显示器信息
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);

    // 获取所有任务栏句柄
    EnumWindows(EnumWindowsProc, 0);

    // 按显示器顺序对任务栏排序
    std::sort(taskbars.begin(), taskbars.end(), CompareTaskbarByMonitorOrder);

    //保存任务栏句柄
    for (const auto& taskbar : taskbars)
    {
        secondary_taskbars.push_back(taskbar.hwnd);
    }
}

int CTaskbarHelper::GetDisplayNum()
{
    monitors.clear();
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
    return static_cast<int>(monitors.size());
}

int CTaskbarHelper::GetSecondaryTaskbarNum()
{
    taskbars.clear();
    EnumWindows(EnumWindowsProc, 0);
    return static_cast<int>(taskbars.size());
}
