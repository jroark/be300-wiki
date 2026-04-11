# Source Layout

The emulator codebase is organized into three main areas: core emulation logic, hardware peripheral models, and the GXemul CPU engine submodule.

## src/ -- Core Emulation

| File | Purpose |
|------|---------|
| `machine_be300.c` | Machine setup: GXemul framework initialization, VR4131 CPU configuration, device registration, kernel loading, and main emulation loop |
| `be300.h` | Central state struct (`be300_state_t`), machine configuration (`machine_config_t`), physical address map constants |
| `loader.c` / `loader.h` | ELF kernel loader for Linux boots; NAND image file reader for WinCE boots |
| `ui.c` / `ui.h` | SDL2 display rendering, input handling, screenshot capture |
| `main.c` | CLI argument parsing; calls `be300_create`, `be300_run`, `be300_destroy` |
| `host_io.c` / `host_io.h` | Host I/O handling |
| `wince_boot.c` / `wince_boot.h` | WinCE cold-boot vector tracking and boot checkpoint logging |
| `wince_boot_types.h` | WinCE boot state machine flags and definitions |
| `be300_devices.c` | VRC4173 latch register, PPSH parallel port debug shell, WinCE auxiliary device |
| `ppsh.h` | PPSH protocol definitions (sync words, message types, framing) |

## src/hw/ -- Hardware Peripherals

Each peripheral is modeled as a pair of `.c` and `.h` files handling register reads, writes, and device behavior:

| File | Peripheral |
|------|-----------|
| `bcu.c` / `bcu.h` | Bus Control Unit -- bus arbitration, chip select, revision registers |
| `icu.c` / `icu.h` | Interrupt Control Unit -- interrupt status, mask, and routing |
| `rtc.c` / `rtc.h` | Real-Time Clock -- elapsed time registers, RTCL1 periodic timer, RTCINTREG write-one-to-clear |
| `siu.c` / `siu.h` | Serial Interface Unit -- UART configuration and control |
| `nand.c` / `nand.h` | NAND flash controller -- command/address registers, page read engine, OOB synthesis, ECC |
| `gpio.c` / `gpio.h` | General-purpose I/O -- pin direction, data, interrupt configuration |
| `cmu.c` / `cmu.h` | Clock Management Unit -- clock enables and dividers |
| `pmu.c` / `pmu.h` | Power Management Unit -- power state, standby control |

## gxemul/ -- CPU Engine (Git Submodule)

The GXemul 0.7.0 engine is included as a Git submodule from the `jroark/GXemul` repository (`be300` branch). It provides the MIPS CPU core and several native device models.

### gxemul/src/cpus/

VR4131 MIPS CPU implementation:

- Instruction execution (MIPS32 + MIPS16)
- CP0 coprocessor (system control registers)
- TLB management (32 entries, variable page sizes)
- dyntrans dynamic binary translation
- Exception and interrupt delivery

### gxemul/src/core/

Framework services:

- Memory subsystem (physical and virtual address translation)
- Interrupt controller framework
- Timer infrastructure
- Settings and configuration
- Console I/O framework

### gxemul/src/devices/

Native device models used by the BE-300 machine:

| Device | File | Function |
|--------|------|----------|
| VR41xx | `dev_vr41xx.c` | VR4131 ICU, timer, GPIO, interrupt assertion/deassertion |
| NS16550 | `dev_ns16550.c` | VRC4173 SIU UART (8250/16550-compatible) |
| Framebuffer | `dev_fb.c` | Generic framebuffer device |
| RAM | `dev_ram.c` | RAM and ROM memory regions |

### gxemul/config.h

MIPS-only build configuration. Unnecessary architecture support (ARM, PowerPC, etc.) is stripped from the build.
