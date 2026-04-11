# Hardware Overview

The Casio BE-300 is built around a two-chip architecture: the **NEC VR4131** system-on-chip provides the MIPS CPU and core peripherals, while the **NEC VRC4173** companion chip handles video, touchscreen, CompactFlash, serial, and other I/O functions.

## System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    NEC VR4131 SoC                        │
│  MIPS32 LE @ 166 MHz, 280 MIPS, PRId 0x00000C81        │
│                                                         │
│  On-chip: BCU, CMU, ICU, PMU, RTC, GIU, SIU, DSIU,    │
│           PCI, SDRAMU, SCU                              │
│  Peripheral I/O base: 0x0F000000                        │
└───────────┬─────────────────────────────────────────────┘
            │ Local bus / PCI
┌───────────┴─────────────────────────────────────────────┐
│               NEC VRC4173 Companion Chip                 │
│  Board ID 0x7100, I/O base: 0x0A000000                  │
│                                                         │
│  Peripherals: PIU (touchscreen), CF, SIU (serial),      │
│  GPIO, buzzer, CMU, video controller                    │
│  Framebuffer: 0x0A200000                                │
│  NAND controller: 0x0A000C00+                           │
└─────────────────────────────────────────────────────────┘
```

## Key Components

| Component | Part Number | Description |
|---|---|---|
| CPU/SoC | NEC VR4131 | MIPS32 little-endian, 166 MHz, 280 MIPS |
| Companion Chip | NEC VRC4173 | Peripheral controller, Board ID 0x7100 |
| SDRAM | Samsung K4S641633D (x2) | 16 MB total (two 8 MB chips) |
| NAND Flash | Samsung K9F2808U0B | 16 MB, 1004 blocks, 32 pages/block, 512 bytes/page |
| LCD Controller | EPSON L50753 | Drives 320x240 32768-color STN panel |
| Boot ROM | Masked ROM | 16 KB at PA 0x1FC00000 |
| Custom Chip | Casio CAK2007 | Proprietary Casio logic |

## Connectors

| Connector | Part | Function |
|---|---|---|
| AC Jack | Hosiden HEC3654 | Power input |
| Headphone | HSJ1636 | Audio output |
| RS-232C | TCX3110 | Serial communication |
| CompactFlash | -- | Type I CF slot |

## Memory

The system has 16 MB of SDRAM mapped at physical address `0x00000000` through `0x00FFFFFF`. The NAND flash is not memory-mapped; it is accessed through the VRC4173 NAND controller registers at `0x0A000C00`. The 16 KB boot ROM is at `0x1FC00000` (accessible via kseg1 at `0xBFC00000`).

See [Memory Map](memory-map.md) for the complete physical address layout.

## Board Photos

Photographs of the BE-300 mainboard, LCD assembly, and component identification are available on the [Board Photos](board-photos.md) page.

## Section Index

- [NEC VR4131 CPU](vr4131.md) -- SoC architecture, CP0, TLB, caches, on-chip peripherals
- [NEC VRC4173 Companion Chip](vrc4173.md) -- Peripheral controller, register spaces
- [Memory Map](memory-map.md) -- Complete physical address map
- [Register Reference](registers.md) -- VR4131 register comparison across chip revisions
- [Interrupts](interrupts.md) -- Full interrupt cascade and handler routing
- [NAND Flash](nand.md) -- Flash geometry, controller, OOB format, read sequence
- [Serial Port](serial.md) -- UART configuration
- [Framebuffer](framebuffer.md) -- Video memory layout and pixel format
- [Touchscreen](touchscreen.md) -- PIU registers and interrupt path
- [CompactFlash](cf-socket.md) -- CF socket and PCMCIA interrupts
- [GPIO & Buttons](gpio.md) -- GIU registers and hardware buttons
- [Board Photos](board-photos.md) -- PCB photographs
