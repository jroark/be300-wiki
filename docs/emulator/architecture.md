# Architecture

The BE-300 emulator combines a modified GXemul 0.7.0 CPU engine with custom hardware peripheral emulation to reproduce the Casio BE-300's system architecture.

## Target Hardware

- **CPU:** NEC VR4131 (MIPS32 little-endian, PRId 0x00000C81)
- **RAM:** 16 MiB SDRAM at PA 0x00000000
- **Companion Chip:** NEC VRC4173 (I/O, interrupts, NAND, audio, touchscreen)
- **Display:** 240x320 pixel, 16-bit color framebuffer
- **Storage:** Samsung K9F2808U0B NAND flash (16 MB)

## CPU Engine

The GXemul 0.7.0 engine provides the core MIPS execution environment:

- **Instruction execution** — Full MIPS32 instruction set plus MIPS16 compact encoding
- **CP0 coprocessor** — System control registers (Status, Cause, EPC, Config, PRId, etc.)
- **TLB** — 32-entry Translation Lookaside Buffer with variable page sizes (4KB-16MB)
- **Exception handling** — TLB refill, general exceptions, cache errors, interrupts
- **dyntrans JIT** — Dynamic binary translation for improved execution speed

## MIPS Address Spaces

The VR4131 uses standard MIPS address segmentation:

| Segment | Virtual Address Range | Translation | Caching |
|---------|----------------------|-------------|---------|
| kuseg | `0x00000000`-`0x7FFFFFFF` | TLB-mapped | Per-TLB entry |
| kseg0 | `0x80000000`-`0x9FFFFFFF` | PA = VA - 0x80000000 | Cached |
| kseg1 | `0xA0000000`-`0xBFFFFFFF` | PA = VA - 0xA0000000 | Uncached |
| kseg2 | `0xC0000000`-`0xFFFFFFFF` | TLB-mapped | Per-TLB entry |

Most kernel code and data lives in kseg0 (cached). Hardware registers are accessed through kseg1 (uncached). User-mode code runs in kuseg with TLB translation.

## Physical Memory Map

| Physical Address | Size | Device |
|-----------------|------|--------|
| `0x00000000` | 16 MiB | SDRAM |
| `0x0A000000` | 64 KiB | VRC4173 I/O registers |
| `0x0A200000` | 256 KiB | Framebuffer |
| `0x0C000000` | — | PPSH debug interface |
| `0x0F000000` | 64 KiB | VR4131 SoC registers (ICU, timer, GPIO) |
| `0x1FC00000` | 16 KiB | Boot ROM |

## Interrupt Controller

The VR4131 Interrupt Control Unit (ICU) manages interrupt routing between peripherals and the CPU:

- Status and mask registers for each interrupt source
- Write-one-to-clear semantics for interrupt acknowledgment
- Parent IRQ line assertion to the CPU when any unmasked interrupt is pending
- VRC4173 cascaded interrupts routed through the VR4131 ICU

## Timer

The RTC periodic interrupt provides the system tick:

- 100 Hz periodic interrupt (RTCINTREG)
- Write-one-to-clear for interrupt acknowledgment
- Used by both Linux and WinCE for scheduling and timekeeping

## UART

An 8250/16550-compatible serial port at PA `0x0A008680` (VRC4173 SIU):

- Directly handled by GXemul's `dev_ns16550` device
- Linux kernels use it as `ttyS0` for serial console output
- WinCE uses it for debug output during boot

## Display

The framebuffer is mapped at PA `0x0A200000` (VA `0xAA200000` in kseg1):

- 240x320 pixels, 16-bit color (RGB565)
- Rendered to an SDL2 window on the host
- WinCE renders "Starting..." and "Initializing..." text at runtime via GWE font engine
- Linux kernels use a standard framebuffer driver

## Boot ABI

The emulator supports two boot paths:

**Linux kernel boot (`--kernel`):**

- Loads ELF binary into memory
- Sets up argc/argv in MIPS registers (`$a0`/`$a1`)
- Jumps to ELF entry point

**WinCE cold boot (`--nand`):**

- Starts at ROM reset vector (VA `0xBFC00000`)
- ROM initializes hardware, reads NAND, loads SPL
- SPL decompresses NK.exe into RAM
- ROM's MIPS16 boot dispatcher sets up callback tables
- ROM jumps to NK.exe entry point (VA `0x80076B50`)
