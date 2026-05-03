# Kernel

## Overview

Linux4.BE used the Linux 2.4.18 kernel as its base, adapted for the NEC VR4131 SoC in the Casio BE-300. A Linux 2.6.8.1 port was later contributed by Mouse.

## Supported Hardware

The following hardware was supported by the original Linux4.BE kernels:

- **Serial console** (VR4131 SIU UART at 0x0A008680)
- **Framebuffer** (240x320 portrait, 16-bit color in the current emulator model)
- **Ramdisk boot** (initrd loaded by CyaCE)

## Not Supported

The following hardware was **not** supported in the original kernels:

- NAND flash (no read/write access to internal storage)
- Touchscreen (not exposed as a Linux input device)
- CompactFlash (no Linux CF driver for the VR4131 socket)
- Sound

!!! info "Warm Start Caveat"
    The Linux kernels were originally loaded from a running WinCE environment via the CyaCE bootloader (warm start). Hardware may have been partially initialized by WinCE before the kernel took control.

## Available Kernel Builds

| Kernel | Version | Description |
|--------|---------|-------------|
| `vmlinux-pgui-demo` | 2.4.18 | PicoGUI demo --- known good, boots to userspace on real hardware |
| `vmlinux` | 2.4.18 | Base kernel, not stripped, includes debug symbols |
| `vmlinux-mw` | 2.4.18 | MicroWindows/Nano-X build |
| `vmlinux-pgui-test1` | 2.4.18 | PicoGUI test build |
| `vmlinux-qte` | 2.4.18 | Qt/Embedded build |
| `vmlinux_sdlregtest` | 2.4.18 | SDL register test |
| `vmlinux-2.6` | 2.6.8.1 | Linux 2.6 port by Mouse |

The reference kernel for testing is `vmlinux-pgui-demo`, which is confirmed to boot to a working PicoGUI userspace on real hardware.

## Linux 2.6 Port

Mouse contributed a Linux 2.6.8.1 port (`linux2.6-l4be.diff`). Key differences from the 2.4 kernels:

- `prom_init()` takes no parameters; `arcs_cmdline` is not populated from bootloader arguments
- Bootloader arguments were not consumed by the 2.6 `prom_init()` path
- Serial console is registered directly via `be300_console_init()` in `prom_init` (UART at 0xAA008680)
- The framebuffer color interpretation differs from the 2.4 kernels; older
  emulator builds carried a compatibility flag for this, but the current
  source CLI is focused on WinCE NAND/ROM paths.

## Building from Source

The kernel source is available from the project CVS repository:

```bash
cvs -d:pserver:anonymous@cvs.linux4.be:/cvsroot co linux/kernel
```

Build steps:

```bash
make mrproper
cp config_be300 .config
make oldconfig
make dep
make clean
make
```

This requires a MIPS little-endian cross-compiler (`mipsel-linux-gcc`). See the [Toolchain](toolchain.md) page for setup instructions.

## Kernel Command Line

The standard command line used for BE-300 kernels:

```
console=tty0 console=ttyS0,9600 root=/dev/ram
```

- `console=tty0` --- output to framebuffer
- `console=ttyS0,9600` --- output to serial port at 9600 baud
- `root=/dev/ram` --- boot from ramdisk

!!! warning "Serial Console Behavior"
    Using `console=ttyS0` as the primary console can cause userspace to hang when processes write to the serial driver. For emulator testing, use `console=tty0` as the primary console.
