# Architecture

The emulator combines a patched GXemul MIPS CPU engine with BE-300-specific
device models. The board model is physical-address based: MIPS kseg0/kseg1
aliases are handled by the CPU/memory core, while devices are registered at the
physical addresses used by real hardware.

## Target Hardware

| Component | Emulated behavior |
|-----------|-------------------|
| CPU | NEC VR4131-class MIPS, little-endian, CP0/TLB/MMU, MIPS16 for ROM code |
| RAM | 16 MiB SDRAM default at PA `0x00000000` |
| ROM | 16 KB reset ROM at PA `0x1FC00000`, entered through VA `0xBFC00000` |
| Companion | NEC VRC4173 register windows at PA `0x0A000000` |
| Display | 240x320 guest framebuffer at PA `0x0A200000`, 256-pixel stride |
| Storage | 16 MB NAND plus optional ATA CompactFlash images |
| Serial | VRC4173 SIU UART at PA `0x0A008680`, 8250/16550-compatible |

## Boot Paths

### WinCE NAND Cold Boot

The primary path starts from the ROM reset vector and does not patch guest
binaries or force handoff state:

```text
0xBFC00000 ROM reset
  -> ROM CPU/cache/platform setup
  -> ROM NAND reads
  -> SPL/Kloader from NAND
  -> NK.exe decompression
  -> WinCE kernel entry
  -> first-boot UI and shell paths
```

Native cold boots leave the RTC in the default first-boot state unless
`--rtc-host-time` is supplied.

### ROM Image Path

A positional ROM image can be loaded at the reset ROM window for targeted ROM
experiments. It is mutually exclusive with `--nand` and `--restore`.

### Restore Path

`--restore --cf <image>` enters the CompactFlash recovery path used by
NANDWRITER-style restore flows. This path uses the primary PCMCIA socket and
cannot be combined with `--ne2000`.

## Physical Memory Map

| Physical Address | Size | Device |
|------------------|------|--------|
| `0x00000000` | 16 MiB default | SDRAM |
| `0x0A000000` | VRC4173 windows | Companion-chip registers, PIU, GIU, latches |
| `0x0A008680` | UART window | VRC4173 SIU / COM1 |
| `0x0A200000` | Framebuffer | LCD framebuffer backing store |
| `0x0C000120` | PPSH data/status | Parallel Port Shell probe/debug transport |
| `0x0F000000` | VR4131 I/O | BCU, CMU, PMU, ICU, RTC, GPIO |
| `0x1FC00000` | 16 KiB active ROM | Reset vector and masked boot ROM |

## CPU And MMU

GXemul provides the MIPS execution core:

- MIPS32 instruction execution
- MIPS16 interpreter for the ROM's compact code sections
- CP0 system registers, exceptions, and interrupt delivery
- 32-entry TLB with variable page sizes
- kseg0/kseg1 direct-mapped translation
- dynamic translation for performance where supported

The BE-300 ROM uses cross-mode MIPS32/MIPS16 calls, including `JALX`, during
NAND loading and boot dispatch. NK.exe itself is MIPS32.

## Interrupts And Timing

The VR4131 ICU and VRC4173 cascades are modeled enough for WinCE driver and
OAL paths:

- Interrupt pending/mask registers use write-one-to-clear where hardware does.
- The PIU touch path routes through the GIU cascade and PIUINTREG.
- CompactFlash/PCMCIA state and level events route through the GIRQ0 cascade.
- CP0 Compare delivery is driven by emulated instruction progress rather than
  host wall-clock SIGALRM cadence, which keeps scheduler and touch behavior
  deterministic under tracing.

## Display And Input

The guest-visible framebuffer is 240x320 with a 256-pixel stride. The native
SDL frontend can show either the LCD alone or the LCD inside the BE-300 frame.

The touchscreen model implements the VRC4173 PIU scan sequencer used by WinCE:

- WaitPenTouch, PenDataScan, and IntervalNextScan state progression
- page 0/page 1 coordinate buffers
- page-valid interrupts, data-lost interrupt, and W1C acknowledgement
- conversion timing tied to emulated CP0 Count cycles

## Serial Peers

COM1 is the VRC4173 SIU UART. Several emulator features intentionally share
that one guest-visible UART and are mutually exclusive:

- PPSH debug shell probe
- PC Connect time-sync peer
- PC Connect host chardev bridge
- Stowaway keyboard dock

This matches the hardware model: these are different peers on the same serial
path, not independent guest devices.
