# Downloads

## Kernels

| File | Version | Description |
|------|---------|-------------|
| `vmlinux-pgui-demo` | 2.4.18 | PicoGUI demo --- known good, boots to userspace on real hardware |
| `vmlinux` | 2.4.18 | Base kernel with debug symbols, not stripped |
| `vmlinux-mw` | 2.4.18 | MicroWindows/Nano-X build |
| `vmlinux-pgui-test1` | 2.4.18 | PicoGUI test build |
| `vmlinux-qte` | 2.4.18 | Qt/Embedded build |
| `vmlinux_sdlregtest` | 2.4.18 | SDL register test |
| `vmlinux-2.6` | 2.6.8.1 | Linux 2.6 port by Mouse |

All kernels are ELF 32-bit LSB executables for MIPS little-endian, statically linked. They contain embedded ramdisks and are loaded by the CyaCE bootloader.

## Ramdisks

| File | Description |
|------|-------------|
| `ramdisk-pgui-full.gz` | Full PicoGUI environment with virtual keyboard and demo apps (727 KB) |
| `ramdisk-pgui-shell.gz` | PicoGUI with shell access |
| `orig_ramdisk.gz` | Original minimal ramdisk (BusyBox only) |

Ramdisks are gzip-compressed ext2 filesystem images. Place them at `arch/mips/ramdisk/ramdisk.gz` in the kernel source tree to embed in a kernel build.

## Tools

| Tool | Description |
|------|-------------|
| **CyaCE bootloader** | Modified CyaCE loader for BE-300 (source and WinCE binary). Loads Linux from CompactFlash card without ROM modification. |
| **InspectorTux** | WinCE device inspection tool for probing BE-300 hardware registers, memory layout, and system information. |

## Documentation

| Document | Description |
|----------|-------------|
| **VR4131 User Manual** (PDF) | NEC VR4131 SoC datasheet and programmer reference |
| **VRC4173 User Manual** (PDF) | NEC VRC4173 companion chip datasheet |
| **Driver Design Guidelines for BE-300** (PDF) | Casio development guidelines for the BE-300 platform |

## Source Code

The complete source code was hosted in the project CVS repository:

```
cvs -d:pserver:anonymous@cvs.linux4.be:/cvsroot co linux/kernel
```

Key source directories:

| Path | Contents |
|------|----------|
| `linux/kernel` | Linux 2.4.18 kernel with BE-300 platform support |
| `cyace/` | CyaCE bootloader source |
| `linux2.6-l4be.diff` | Patches for Linux 2.6.8.1 BE-300 port |

!!! note "Availability"
    The original linux4.be servers are no longer online. The project website has been preserved from archive.org and is available in the [archive section](/static/archive/linux4be/) of this wiki. Kernel binaries, ramdisks, and tools referenced above are preserved in the emulator project repository.
