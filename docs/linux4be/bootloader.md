# Bootloader

## CyaCE

Linux4.BE uses a modified version of the CyaCE (Compact yet another CE) loader to boot Linux on the BE-300. CyaCE is a small Windows CE application that loads a Linux kernel and ramdisk into memory, then transfers control to the kernel entry point.

## How It Works

CyaCE runs as a normal WinCE application. It reads a configuration file, loads the specified kernel ELF binary and ramdisk into RAM, sets up the kernel command line, and jumps to the kernel entry point. Because it runs from within WinCE, no ROM flashing or permanent device modification is required.

!!! note "Warm Start"
    CyaCE performs a warm start --- the hardware has already been initialized by WinCE before the Linux kernel takes over. This means some hardware state (clocks, memory timings, peripherals) is pre-configured. The Linux kernel does not need to perform full cold-boot hardware initialization.

## Installation

Place `AutoRun.exe` (the CyaCE binary) in the following path on a CompactFlash card:

```
\Storage Card\CE\R4100\AutoRun.exe
```

When the CF card is inserted into the BE-300, WinCE automatically executes this file.

Alternatively, place CyaCE anywhere on the CF card and launch it manually from the WinCE file explorer.

## Configuration

CyaCE reads a configuration file that defines one or more boot entries:

```
# Linux4.BE boot configuration
timeout=300

image=vmlinux-pgui-demo
label=picogui
append=console=tty0 root=/dev/ram

image=vmlinux
label=linux4be
append=console=tty0 console=ttyS0,9600 root=/dev/ram

image=vmlinux
label=linux4be-serial
append=console=ttyS0,9600 root=/dev/ram
```

Each entry consists of:

- `image=` --- kernel ELF binary filename (relative to the CF card)
- `label=` --- display name shown in the boot menu
- `append=` --- kernel command line arguments

The `timeout=` directive sets the auto-boot delay in deciseconds. Lines beginning with `#` are comments.

## Non-Destructive Boot

The key advantage of CyaCE is that it requires no changes to the device ROM or NAND flash. The entire Linux environment lives on the CompactFlash card. To return to WinCE, press the hardware reset button. The device reboots normally into Windows CE with no trace of the Linux session.

## Source Code

The CyaCE source code is available in the project CVS repository under `cyace/`. The BE-300 version includes modifications for the VR4131 platform, including correct memory mapping for kernel loading.
