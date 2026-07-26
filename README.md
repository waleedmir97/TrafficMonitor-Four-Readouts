# Performance Monitor

A focused Windows 11 taskbar fork of
[TrafficMonitor V1.86](https://github.com/zhongyang219/TrafficMonitor/tree/V1.86).
It displays exactly five live measurements:

- Upload speed
- Download speed
- CPU usage
- RAM usage
- GPU usage

The widget uses a fixed two-row text layout in the taskbar. It has no floating
main window, no notification-area icon, no graph, and no traffic-history
recording or export.

## Current release

`v1.4.0` is the current x64 build. On Windows 11 it tries to sit immediately
to the right of the Windows Weather/Widgets control. If that control is hidden,
unavailable, or leaves insufficient room, the app uses TrafficMonitor's
existing non-overlapping taskbar fallback. The widget follows the taskbar
through full-screen transitions and debounces temporary Weather lookup failures
so shell activity cannot make it jump or disappear.

Download the current ZIP from
[GitHub Releases](https://github.com/waleedmir97/Performance-Monitor/releases/latest).
Its SHA-256 is:

```text
8C43585ADDD3AACF1CC9A2229CE7C36C73110998A335A5B7243C8875189A842A
```

The executable inside has SHA-256:

```text
BD653D5F5653D18F019B26B6E53B03E0932B8290DACAB83E959766D06EAEAD3C
```

## Install and use

1. Download and extract the latest release ZIP to a permanent folder.
2. Run `PerformanceMonitor.exe`.
3. Launch the same executable again whenever you need the compact control
   window.

The app starts automatically at Windows sign-in by default. The compact
settings window has a `Start automatically when I sign in to Windows`
checkbox that can disable or re-enable this behavior. Its recovery button
closes and recreates the taskbar widget; there is no hidden main window to
show.

The Microsoft Visual C++ 2015-2022 x64 runtime may be required on a PC that
does not already have it.

## Metric model

Performance Monitor publishes one coherent live snapshot approximately once
per second. It favors accurate, low-overhead measurement of the underlying
resource over synchronization with another application's display refresh:

- CPU is total processor busy time.
- RAM is used physical memory as a percentage of usable physical memory.
- GPU is the busiest physical GPU engine after process instances are grouped.
- Upload and download use 64-bit counters from the active default-route
  interface.

## Privacy and persistence

Network, CPU, RAM, and GPU readings are sampled for the live display only. The
custom runtime path does not load, update, save, or back up traffic-history
data, and it does not export CSV files.

The only custom persistent preference is the per-user automatic-start choice:

```text
HKEY_CURRENT_USER\Software\PerformanceMonitor\AutoStartEnabled
```

## Build

Requirements:

- Visual Studio 2022 Build Tools or Visual Studio 2022
- Desktop development with C++
- C++ MFC components
- A current Windows SDK

Build the hardware-monitoring dependency before the application:

```powershell
$env:SolutionDir = "$PWD\"
msbuild OpenHardwareMonitorApi\OpenHardwareMonitorApi.vcxproj /m:1 /t:Build /p:Configuration=Release /p:Platform=x64
msbuild TrafficMonitor\TrafficMonitor.vcxproj /m:1 /t:Build /p:Configuration="Release (lite)" /p:Platform=x64
```

The executable is written to:

```text
Bin\x64\Release (lite)\PerformanceMonitor.exe
```

## Version history

Five compiled builds are preserved as releases:

- `v1.0.0`: fixed taskbar-only, text-only four-readout build
- `v1.1.0`: compact settings, default automatic startup, and widget recovery
- `v1.2.0`: Windows 11 Weather/Widgets anchoring with a compatibility fallback
- `v1.3.0`: stable taskbar attachment, full-screen behavior, and anchor recovery
- `v1.4.0`: Performance Monitor identity, RAM, and higher-fidelity live metrics

See [RELEASES.md](RELEASES.md) for dates and verified hashes.

The exact intermediate source snapshots for `v1.0.0` and `v1.1.0` were not
retained when those binaries were built. Their original ZIPs are preserved
unchanged as historical release assets. The repository contains the exact
source for `v1.2.0` through `v1.4.0`, based on the upstream `V1.86` commit
history.

## Upstream and license

This project is a derivative of
[zhongyang219/TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor).
The upstream English documentation remains available in
[README_en-us.md](README_en-us.md).

The upstream copyright notice and Anti 996 License are preserved in
[LICENSE](LICENSE). Third-party components retain their own notices.
