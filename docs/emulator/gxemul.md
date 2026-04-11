# GXemul Integration

The BE-300 emulator uses a modified version of [GXemul 0.7.0](https://gavare.se/gxemul/) by Anders Gavare as its CPU engine. GXemul is included as a Git submodule and configured for MIPS-only operation.

## Submodule Setup

The GXemul source lives in the `gxemul/` directory, tracking the `be300` branch of `jroark/GXemul`. After cloning, initialize the submodule:

```bash
git submodule update --init
```

## What GXemul Provides

The engine handles the core CPU simulation:

- **Instruction execution** -- Full MIPS32 instruction set
- **CP0 coprocessor** -- All system control registers (Status, Cause, EPC, Config, PRId, EntryHi/Lo, PageMask, Wired, Count/Compare, etc.)
- **TLB** -- 32-entry Translation Lookaside Buffer with variable page sizes (4KB to 16MB)
- **Exception handling** -- TLB refill, general exceptions, cache errors, interrupt delivery
- **dyntrans JIT** -- Dynamic binary translation converts frequently executed MIPS code into host-native operations for improved performance
- **kseg0/kseg1 translation** -- Hardware address segment mapping without TLB involvement

## MIPS16 Interpreter

A full MIPS16 interpreter was added to GXemul specifically for BE-300 boot ROM support.

The BE-300's 16KB boot ROM contains approximately 5.5KB of MIPS16 code -- 34 functions located at ROM offsets `0xC20` through `0x219B`. These functions handle critical boot tasks including NAND flash reading, B000FF section copying, and the boot dispatcher that populates OEMInit callback tables before handing off to NK.exe.

Key details of MIPS16 usage in the boot ROM:

- MIPS16 functions use **JALX** (Jump And Link Exchange) instructions to call back into MIPS32 ROM helper routines, creating a cross-mode call graph
- The BEV exception handler at ROM offset `+0x380` does `JALR` to `0x9FC00C85` (bit 0 set = MIPS16 mode switch)
- MIPS32 helpers called from MIPS16 include memory copy, memory set, context save, and the mailbox writer that stores NK.exe's entry point

!!! info "NK.exe is pure MIPS32"
    The MIPS16 interpreter is only needed for the boot ROM. NK.exe (the WinCE kernel) is 100% MIPS32 with no MIPS16 code anywhere in the 6.2MB image.

## Native Devices

GXemul includes several device models used by the BE-300 emulation:

| Device | Source File | Function |
|--------|------------|----------|
| `dev_vr41xx` | `gxemul/src/devices/dev_vr41xx.c` | VR4131 ICU, timer, GPIO; manages interrupt assertion/deassertion and the `timer_tick()` mechanism |
| `dev_ns16550` | `gxemul/src/devices/dev_ns16550.c` | 8250/16550-compatible UART at VRC4173 SIU address `0x0A008680` |
| `dev_fb` | `gxemul/src/devices/dev_fb.c` | Generic framebuffer for display output |
| `dev_ram` | `gxemul/src/devices/dev_ram.c` | RAM and ROM memory region backing |

## Build Configuration

The file `gxemul/config.h` configures a MIPS-only build, stripping support for ARM, PowerPC, SPARC, and other architectures that are not needed. This reduces build time and binary size.

## Modifications from Upstream

The `be300` branch includes several modifications to the upstream GXemul codebase:

- MIPS16 instruction interpreter (not present in upstream GXemul)
- BE-300 machine definition in `src/machines/machine_hpcmips.c`
- Fixes for MIPS16-specific instruction decode bugs (MOV32R/MOVR32 register encoding, RRIA EXTEND immediate format)
- MIPS-only build configuration
- Minimal stubs for unused subsystems (debugger, X11, disk images) in `gxemul/src/stubs.c`
