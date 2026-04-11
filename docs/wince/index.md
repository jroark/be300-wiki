# Windows CE Overview

The Casio BE-300 ships with **Windows CE 3.0** as its stock operating system. Some later models and firmware updates use **Windows CE 4.0** (also known as .NET). The entire OS, bootloader, and user filesystem are stored on 16MB of onboard NAND flash.

## Stock Firmware

The BE-300's firmware consists of four partitions on NAND flash:

| Partition | Contents | Size |
|-----------|----------|------|
| Boot metadata | Partition table, boot flags | 16 KB |
| SPL / Kloader | Second-stage bootloader (B000FF format) | 64 KB |
| NK.exe | Compressed WinCE kernel image | ~3.6 MB |
| FAT16 filesystem | User data and system files | ~12 MB |

The kernel image (NK.exe) contains 95 XIP (execute-in-place) modules that make up the WinCE operating system, including the shell, device drivers, and the GWE (Graphics, Windowing, and Events) subsystem.

## Boot Sequence

A cold boot (from full power-off, as after battery removal) follows this chain:

```
Boot ROM (0xBFC00000)
  --> CP0 init, SDRAM timing, clock setup
  --> Read NAND, load SPL into RAM

SPL / Kloader (0x80F00000)
  --> HW init (VR4131, VRC4173)
  --> Read NK.exe from NAND
  --> Decompress into RAM at 0x80060000
  --> Write entry address to mailbox (PA 0x24FC)

Boot ROM (continued)
  --> BINFS section copier (copies .data/.bss)
  --> Boot dispatcher (registers OEMInit callbacks)
  --> Jump to NK.exe entry point

NK.exe (0x80076B50)
  --> OAL hardware initialization
  --> Exception handler installation
  --> kernel_init(pTOC) -- loads 95 XIP modules
  --> Scheduler starts
  --> Shell launches
```

## Cold Boot Display Sequence

After a cold boot, the user sees:

1. **"Initializing..."** with a progress bar that fills up (NK.exe pre-init phase)
2. **"Starting..."** displayed briefly (post-init, shell loading)
3. **Touch calibration screen** (first boot or after factory reset)
4. **WinCE desktop**

!!! note
    The "Starting..." and "Initializing..." strings do not exist anywhere in NK.exe or the NAND image as static text. They are rendered at runtime by the GWE graphics subsystem using font rendering into memory buffers that are zero-filled in the binary.

## Reverse Engineering

The entire boot chain -- from the 16KB masked ROM through SPL decompression and NK.exe kernel initialization -- has been reverse-engineered as part of the emulator project. Key discoveries include:

- The ROM contains both **MIPS32 and MIPS16 code**, with cross-mode calls via JALX
- NK.exe always takes the "warm" restore path due to a register clobber bug, requiring the emulator to seed a resume context with cold-start values
- The PPSH (Parallel Port Shell) debug interface can redirect WinCE console I/O to a debug shell, bypassing the GUI entirely
- The SPL uses a Casio proprietary compression scheme for NK.exe

See the subsections for detailed documentation of each boot stage.
