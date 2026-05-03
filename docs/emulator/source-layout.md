# Source Layout

The emulator repo is organized around a reusable BE-300 machine core, a patched
GXemul MIPS engine, and native/web/Android frontends.

## Core Machine

| Path | Purpose |
|------|---------|
| `src/main.c` | Native CLI option parsing and process entry point |
| `src/machine_be300.c` | Machine creation, ROM/NAND loading, GXemul setup, run loop, RTC host-time setup, shutdown |
| `src/be300.h` | Main state structs, physical address constants, public embeddable API |
| `src/be300_devices.c` | VRC4173 board glue, input devices, latches, PPSH, PCMCIA routing, buzzer, WinCE auxiliary devices |
| `src/be300_probe.c` | MMIO coverage and stall-detection instrumentation helpers |
| `src/host_io.c` | Host stdin/nonblocking I/O helpers |
| `src/ui.c` | SDL2 display/audio/input frontend, frame asset support, screenshot and frame callbacks |
| `src/pcconnect.c` | UART backend dispatch shared by PC Connect peers |
| `src/pcconnect_bridge.c` | TCP/Unix/PTY bridge, cable gating, byte tee, baud throttle |
| `src/pcconnect_ring.h` | Shared ring buffer for serial bridge paths |
| `src/stowaway.c` | Targus/Stowaway serial keyboard dock model |

## Hardware Models

| Path | Peripheral |
|------|------------|
| `src/hw/bcu.c` | VR4131 Bus Control Unit |
| `src/hw/cmu.c` | Clock Management Unit |
| `src/hw/pmu.c` | Power Management Unit |
| `src/hw/icu.c` | Interrupt Control Unit status, mask, and W1C behavior |
| `src/hw/siu.c` | Serial Interface Unit configuration state |
| `src/hw/rtc.c` | Real-Time Clock and elapsed counter helpers |
| `src/hw/gpio.c` | GPIO registers and interrupt configuration |
| `src/hw/nand.c` | NAND controller, SPL/NK read paths, OOB/ECC synthesis, restore writes |
| `src/hw/cf.c` | PCMCIA/CompactFlash ATA, NANDWRITER boot taskfile, NE2000 model |

## GXemul Engine

`gxemul/` is a MIPS-focused GXemul 0.7.0 tree. The BE-300 build compiles only
the CPU, memory, timer, interrupt, console, symbol, network, and device pieces
needed by this machine.

Important GXemul paths:

| Path | Purpose |
|------|---------|
| `gxemul/src/cpus/cpu_mips.c` | Main MIPS CPU engine and included templates |
| `gxemul/src/cpus/cpu_mips16.c` | MIPS16 interpreter needed by the BE-300 boot ROM |
| `gxemul/src/cpus/cpu_mips_coproc.c` | CP0, TLB, Count/Compare, exception helpers |
| `gxemul/src/core/memory.c` | Physical/virtual memory access and device dispatch |
| `gxemul/src/core/timer.c` | GXemul timer framework |
| `gxemul/src/devices/dev_vr41xx.c` | VR41xx timer/interrupt support used by the board |
| `gxemul/src/devices/dev_ns16550.c` | 8250/16550-compatible VRC4173 SIU UART |
| `gxemul/src/devices/dev_fb.c` | Framebuffer backing store |
| `gxemul/src/net/` | Native networking backend used by NE2000 |

## Frontends

| Path | Purpose |
|------|---------|
| `web/main_web.c` | Emscripten exports for browser worker control |
| `web/` | Browser UI, worker, styles, favicon, static frontend assets |
| `android/app/src/main/cpp/be300_jni.c` | Android JNI bridge into the emulator core |
| `android/app/src/main/java/com/jroark/be300/` | Android activity, view, asset loading, input mapping |
| `src/ui/assets/` | BE-300 frame and button-mask PNG assets shared by native/web/Android |

## Tools And Reference Material

| Path | Purpose |
|------|---------|
| `tools/build_web.sh` | WebAssembly build helper |
| `tools/be300_nand.py` | NAND image inspection/manipulation helpers |
| `tools/inject_stowaway.py` | Build NAND image variant with Stowaway driver registration |
| `tools/pcconnect_diff_tee.py` | Compare bridge tee logs with real PortMon captures |
| `ce/bediag/` | WinCE diagnostic stream driver source |
| `ce/restore_images/` | Restore-image references and recovery utilities |
| `docs/CURRENT_STATUS.md` | Maintained emulator status and investigation guidance |
