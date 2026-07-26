# Release ledger

The hashes below were calculated from the preserved local artifacts before
publication.

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
