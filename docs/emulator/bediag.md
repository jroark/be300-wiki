# BEDiag Tool

BEDiag is a passive Windows CE 3.0 MIPS diagnostic stream driver designed to capture early-state hardware snapshots from a running BE-300. It is not a functional device driver -- its sole purpose is to observe and record system state at specific moments after boot.

## Purpose

BEDiag captures snapshots of memory regions, hardware registers, and system structures that are otherwise difficult to observe. These captures provide ground truth data for validating the emulator's hardware models and boot sequence.

## Source and Build

- **Source:** `ce/bediag/` in the emulator repository
- **Build environment:** eMbedded Visual C++ 3.0 with the Casio BE-300 SDK
- **Platform:** Real BE-300 hardware running stock Windows CE 3.0

## Capture Timing

BEDiag captures state at three points after driver initialization:

1. **At init** -- Immediately when the driver loads
2. **+1 second** -- One second after initialization
3. **+5 seconds** -- Five seconds after initialization

This timing allows observation of how system state evolves during the early post-boot period.

## Captured Regions

Each snapshot includes the following memory regions and register sets:

| Address Range | Contents |
|--------------|----------|
| PA `0x2200`-`0x22FF` | Resume context (GPR/CP0 save area) |
| PA `0x6000`-`0x6FFF` | Boot context |
| PA `0x51680`-`0x51AFF` | OEMInit callback table |
| PA `0x660000` | Object pointers |
| PA `0x66BFC0` | Object headers |
| PA `0x679400`-`0x6795FF` | Post-boot module names |
| `0x0A000C00`-`0x0A000C4F` | VRC4173 registers |
| `0x0F000000`-`0x0F00011F` | VR4131 SoC registers |

## Output

BEDiag produces output through two channels:

- **COM1 serial text** -- Human-readable dump of all captured state
- **File capture** -- Optional file output for automated processing

## Additional Features

### Registry Churn Logging

BEDiag monitors Windows CE registry activity during boot, logging changes to:

- `Drivers\Active`
- `Drivers\BuiltIn`
- `StorageManager`

This reveals the order in which device drivers load and register during the boot process.

## Companion Tools

| Tool | Purpose |
|------|---------|
| `BEDiagKick.exe` | Device-manager debugging utility for triggering and managing BEDiag |
| `BDGMini` | Minimal BuiltIn driver variant for lightweight capture scenarios |
