# Emulator Overview

The BE-300 emulator is a GXemul 0.7.0-based system emulator for the Casio
Cassiopeia BE-300. It runs the same VR4131 MIPS CPU model, ROM reset vector,
NAND boot path, and companion-chip peripherals that the stock device depends
on.

The active target is accurate Windows CE 3.0 cold boot from the real 16 KB
masked boot ROM and a raw NAND restore image:

```bash
./be300 --nand ce/restore_images/All_nand_300.bin
```

That path runs:

```text
ROM reset vector -> SPL/Kloader from NAND -> NK.exe decompression -> WinCE 3.0
```

## Current Status

- WinCE 3.0 cold boot reaches the shell / first-boot UI path from the
  unmodified ROM and NAND image.
- Touch input is usable enough to complete calibration and reach applications.
- Native boots default to the cold-boot RTC presentation so WinCE can ask for
  date/time like real hardware after battery removal.
- `--rtc-host-time` initializes the guest RTC from host local time for
  interactive testing.
- CompactFlash, PCMCIA NE2000, Stowaway keyboard, PC Connect bridge, and the
  PPSH debug-shell probe are implemented as targeted accessory paths.
- Linux kernel boot support remains in the codebase for historical and
  regression work, but current CLI and regression focus are WinCE NAND boots.

!!! info "Development rule"
    Guest binaries stay unmodified. The emulator should model missing hardware
    behavior instead of patching WinCE, pre-seeding RAM, or forcing handoff
    state.

## Key Features

| Feature | Details |
|---------|---------|
| CPU | NEC VR4131-class MIPS, GXemul CP0/TLB/MMU, MIPS32 and MIPS16 |
| Boot ROM | 16 KB masked ROM embedded from `docs/hardware/be300_boot_rom.bin` |
| RAM | 16 MiB default SDRAM, configurable with `--sdram` |
| Display | 240x320 guest framebuffer, optional SDL2 host window and BE-300 frame |
| NAND | Samsung K9F2808U0B-style NAND controller model with OOB/ECC synthesis |
| Input | VRC4173 PIU touchscreen, front buttons, optional Stowaway serial keyboard |
| Storage | ATA CompactFlash image support and NANDWRITER restore path |
| Networking | PCMCIA NE2000 model with native and WebAssembly bridge hooks |
| Serial | VRC4173 SIU UART, PPSH probe, PC Connect time-sync peer, PC Connect bridge |
| Frontends | Native CLI/SDL, WebAssembly worker/frontend, Android JNI app |
| Diagnostics | MMIO logging, first-hit MMIO coverage, trace mode, stall detection |

## Screenshots

**WinCE 3.0 NAND Boot**

![WinCE boot](../images/screenshots/All_nand_300.png)

**Linux PicoGUI Desktop**

![Linux PicoGUI](../images/screenshots/vmlinux-pgui-demo.png)
