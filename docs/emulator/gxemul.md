# GXemul Integration

The BE-300 emulator uses a patched, MIPS-focused GXemul 0.7.0 tree as its CPU
and memory engine. The BE-300 code supplies the board setup, ROM/NAND loading,
and peripheral models.

## Submodule

Initialize the `gxemul/` submodule after cloning:

```bash
git submodule update --init
```

The BE-300 build compiles a small source set from GXemul rather than the full
multi-architecture emulator.

## What GXemul Provides

- MIPS CPU execution and dyntrans support
- CP0 registers, exceptions, and interrupt delivery
- 32-entry TLB with variable page sizes
- kseg0/kseg1 translation
- memory/device dispatch
- timer and interrupt framework
- 8250/16550 UART device
- framebuffer backing store
- native network helpers used by NE2000

## MIPS16

The BE-300 boot ROM contains MIPS16 routines, so the GXemul tree includes a
MIPS16 interpreter in `gxemul/src/cpus/cpu_mips16.c`.

The ROM uses MIPS16 for NAND/B000FF boot code and cross-mode helper calls. A
correct MIPS16 implementation is required before WinCE can load SPL/NK through
the real boot path.

!!! info "NK.exe"
    NK.exe is MIPS32 code. MIPS16 support is required by the masked boot ROM,
    not by the WinCE kernel image itself.

## BE-300-Specific GXemul Changes

- MIPS16 instruction interpreter and decode fixes
- VR4131/BE-300-sensitive CP0 and Count/Compare behavior
- MIPS-only build configuration
- Minimal stubs for unused GXemul subsystems
- Integration hooks for the BE-300 UART peers and board devices

## Native Devices Used By The Board

| GXemul device | Function |
|---------------|----------|
| `dev_vr41xx` | VR41xx timer/interrupt plumbing used by the board |
| `dev_ns16550` | VRC4173 SIU UART at PA `0x0A008680` |
| `dev_fb` | Framebuffer backing store |
| `dev_ram` | RAM and ROM memory backing |
