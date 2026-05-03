# Windows CE Overview

The Casio BE-300 ships with Windows CE 3.0 as its stock operating system. Some
later restore images and related models use Windows CE .NET/4.x. The operating
system, bootloader, and user filesystem live on the onboard NAND flash.

## Stock Firmware

The primary WinCE 3.0 restore image used by emulator development is
`All_nand_300.bin`.

| Region | Contents |
|--------|----------|
| Boot metadata | NAND partition table, boot flags, metadata |
| SPL / Kloader | Second-stage loader in B000FF format |
| NK.exe | Compressed WinCE kernel image |
| FAT16 filesystem | System files, drivers, user-visible storage |

The decompressed NK.exe image contains XIP modules for the kernel, shell,
drivers, GWE, and built-in applications.

## Cold Boot Sequence

A battery-removal cold boot follows this chain:

```text
Boot ROM at 0xBFC00000
  -> CP0/cache/platform setup
  -> NAND reads
  -> SPL/Kloader load

SPL / Kloader
  -> hardware setup
  -> NK.exe read from NAND
  -> NK.exe decompression into SDRAM
  -> entry mailbox update

WinCE NK.exe
  -> OAL hardware initialization
  -> exception vectors and kernel memory setup
  -> XIP module initialization
  -> scheduler and shell paths
  -> first-boot UI
```

The emulator now runs this chain from the real ROM and NAND image without guest
patches, RAM seeds, or synthetic handoff state.

## Cold Boot Display Sequence

The stock first-boot path can show:

1. "Initializing..." with a progress bar
2. "Starting..." during shell/display transition
3. date/time setup when the RTC is in the first-boot/default state
4. touch calibration
5. WinCE desktop

!!! note
    The "Starting..." and "Initializing..." strings are rendered at runtime by
    the GWE/display path. They are not stored as ready-made splash bitmaps in
    NK.exe.

## Current Emulator-Relevant Findings

- The historical "Starting..." splash stall and Welcome.exe/launcher
  dependency theories are superseded by current emulator behavior.
- Normal cold boots no longer rely on the legacy resume-context workaround or
  a forced warm-to-cold return path.
- Touch calibration depends on VRC4173 PIU page-buffer, interrupt, and timing
  behavior.
- Native emulator boots default to cold-boot RTC state. `--rtc-host-time`
  supplies host local time for interactive testing.
- PC Connect, PPSH, Stowaway keyboard, and the time-sync peer all share the
  VRC4173 SIU UART from the guest point of view.

See the subsections for ROM, SPL, NK.exe, restore-image, PPSH, and hibernate
details.
