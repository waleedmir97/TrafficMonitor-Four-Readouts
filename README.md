# TrafficMonitor Four Readouts

A focused Windows 11 taskbar fork of
[TrafficMonitor V1.86](https://github.com/zhongyang219/TrafficMonitor/tree/V1.86).
It displays exactly four live measurements:

- Upload speed
- Download speed
- CPU usage
- GPU usage

The widget uses a fixed two-row text layout in the taskbar. It has no floating
main window, no notification-area icon, no graph, and no traffic-history
recording or export.

## Current release

`v1.3.0` is the current x64 build. On Windows 11 it tries to sit immediately
to the right of the Windows Weather/Widgets control. If that control is hidden,
unavailable, or leaves insufficient room, the app uses TrafficMonitor's
existing non-overlapping taskbar fallback. The widget follows the taskbar
through full-screen transitions and debounces temporary Weather lookup failures
so shell activity cannot make it jump or disappear.

Download the current ZIP from
[GitHub Releases](https://github.com/waleedmir97/TrafficMonitor-Four-Readouts/releases/latest).
Its SHA-256 is:

```text
7310EC47539115267813213B27F98957C6D9FD5128A88BE9619D1542EF6A806F
```

The executable inside has SHA-256:

```text
1F84E3B008612AB666FBDE4AB8A7C0D70D378DA56B49A4798AD01A8A58E3A3FB
```

## Install and use

1. Download and extract the latest release ZIP to a permanent folder.
2. Run `TrafficMonitor.exe`.
3. Launch the same executable again whenever you need the compact control
   window.

The app starts automatically at Windows sign-in by default. The compact
settings window has a `Start automatically when I sign in to Windows`
checkbox that can disable or re-enable this behavior. Its recovery button
closes and recreates the taskbar widget; there is no hidden main window to
show.

The Microsoft Visual C++ 2015-2022 x64 runtime may be required on a PC that
does not already have it.

## Privacy and persistence

Network, CPU, and GPU readings are sampled for the live display only. The
custom runtime path does not load, update, save, or back up
`history_traffic.dat`.

The only custom persistent preference is the per-user automatic-start choice:

```text
HKEY_CURRENT_USER\Software\TrafficMonitorFourReadouts\AutoStartEnabled
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
Bin\x64\Release (lite)\TrafficMonitor.exe
```

## Version history

Four compiled builds are preserved as releases:

- `v1.0.0`: fixed taskbar-only, text-only four-readout build
- `v1.1.0`: compact settings, default automatic startup, and widget recovery
- `v1.2.0`: Windows 11 Weather/Widgets anchoring with a compatibility fallback
- `v1.3.0`: stable taskbar attachment, full-screen behavior, and anchor recovery

See [RELEASES.md](RELEASES.md) for dates and verified hashes.

The exact intermediate source snapshots for `v1.0.0` and `v1.1.0` were not
retained when those binaries were built. Their original ZIPs are preserved
unchanged as historical release assets. The repository contains the exact
source for `v1.2.0` and `v1.3.0`, based on the upstream `V1.86` commit history.

## Upstream and license

This project is a derivative of
[zhongyang219/TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor).
The upstream English documentation remains available in
[README_en-us.md](README_en-us.md).

The upstream copyright notice and Anti 996 License are preserved in
[LICENSE](LICENSE). Third-party components retain their own notices.
