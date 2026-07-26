# Release ledger

The hashes below were calculated from the preserved local artifacts before
publication.

## v1.4.0 - Performance Monitor and live metric fidelity

Built 2026-07-26 from the source currently in this repository.

Asset:
`PerformanceMonitor_v1.4.0_x64.zip`

```text
ZIP SHA-256: 8C43585ADDD3AACF1CC9A2229CE7C36C73110998A335A5B7243C8875189A842A
EXE SHA-256: BD653D5F5653D18F019B26B6E53B03E0932B8290DACAB83E959766D06EAEAD3C
```

Changes:

- Renames the product and executable to Performance Monitor.
- Adds RAM to the taskbar for a fixed five-readout layout.
- Publishes CPU, RAM, GPU, upload, and download from one coherent snapshot.
- Uses total processor busy time, physical-memory load, grouped physical GPU
  engines, and 64-bit counters from the active default-route interface.
- Replaces short-interval polling with event-driven one-second sampling and
  skips unused hardware, frequency, disk, and plugin polling in the lite build.
- Migrates the automatic-start preference to the Performance Monitor registry
  key and removes the legacy key after a successful migration.
- Preserves Weather anchoring, full-screen coverage, taskbar recovery, and the
  no-history runtime.

## v1.3.0 - Taskbar lifecycle fixes

Built 2026-07-25 from the source currently in this repository.

Asset:
`TrafficMonitor_V1.86_x64_FourReadouts_LifecycleFix_CodexBuild.zip`

```text
ZIP SHA-256: 7310EC47539115267813213B27F98957C6D9FD5128A88BE9619D1542EF6A806F
EXE SHA-256: 1F84E3B008612AB666FBDE4AB8A7C0D70D378DA56B49A4798AD01A8A58E3A3FB
```

Changes:

- Uses the correct child-window styles when attaching to Explorer's taskbar.
- Hides the compatibility overlay during full-screen use and restores it
  deterministically when the taskbar is available again.
- Keeps the last valid Weather anchor through transient UI Automation failures.
- Separates positioning from visibility so Task Manager cannot accidentally
  redisplay a previously hidden widget.

## v1.2.0 - Weather anchor

Built 2026-07-16 from the source currently in this repository.

Asset:
`TrafficMonitor_V1.86_x64_FourReadouts_WeatherAnchor_CodexBuild.zip`

```text
ZIP SHA-256: 18101807CA53455902EA6C7F42E3D1DA4BF8E846FE21BF43ECDFEE7FF1BE68FA
EXE SHA-256: F181E063EBDD9763CCB1C2F208450943C0EDE0BFDE13BEFF0426FD4C517D5BB9
```

Changes:

- Anchors immediately after the Windows 11 Weather/Widgets control when safe.
- Reflows when the Weather/Widgets rectangle changes.
- Falls back to the established non-overlapping placement when necessary.

## v1.1.0 - Automatic startup

Built 2026-07-13. This is a preserved historical binary; its exact
intermediate source snapshot was not retained.

Asset:
`TrafficMonitor_V1.86_x64_FourReadouts_TextOnly_Autostart_CodexBuild.zip`

```text
ZIP SHA-256: 0275B58EC278666BD6C014E35809D07E622D16BB6EE6BB379741D24000CBB681
EXE SHA-256: 50579E3849F320A93AB775AD448871BAC78782622B41DD52AB5BC74C5F5B05E6
```

Changes:

- Starts automatically at Windows sign-in by default.
- Adds a compact settings window with a persistent opt-out.
- Replaces inherited main-window controls with taskbar-widget recovery.

## v1.0.0 - Text-only four readouts

Built 2026-07-13. This is a preserved historical binary; its exact
intermediate source snapshot was not retained.

Asset:
`TrafficMonitor_V1.86_x64_FourReadouts_TextOnly_CodexBuild.zip`

```text
ZIP SHA-256: 1B7713E0F9ECB42A24E2DD93201BB5E87C6A526109661F4C3D2D4F0D92DF0ECC
EXE SHA-256: 1D3850A01FE5B3E0598DF6A2AC991315805434F4B6F4C779B15055679CCCBF3D
```

Changes:

- Displays upload, download, CPU, and GPU only.
- Uses a fixed two-row taskbar layout.
- Removes the main-window experience and traffic-history runtime path.
- Disables graph rendering.
