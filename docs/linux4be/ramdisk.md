# Ramdisk

## Overview

Because the Linux4.BE kernels had no NAND flash driver, the system boots entirely from a ramdisk loaded into memory by the CyaCE bootloader. The ramdisk contains a minimal Linux root filesystem with BusyBox and optionally a GUI framework.

## Pre-Built Ramdisks

| Ramdisk | Size | Description |
|---------|------|-------------|
| `ramdisk-pgui-full.gz` | 727 KB | Full PicoGUI environment with virtual keyboard and demo applications |
| `ramdisk-pgui-shell.gz` | --- | PicoGUI with shell access |
| `orig_ramdisk.gz` | --- | Original minimal ramdisk (BusyBox only) |

The ramdisk is placed in the kernel source tree at:

```
arch/mips/ramdisk/ramdisk.gz
```

The kernel build system embeds the ramdisk into the kernel image as an initrd.

## Building a Custom Ramdisk

Building a custom ramdisk requires a MIPS little-endian cross-compilation toolchain. The process involves:

### 1. Cross-Compile uClibc

uClibc is a lightweight C library suitable for embedded systems. Cross-compile it for `mipsel-linux` using the toolchain described in the [Toolchain](toolchain.md) page.

### 2. Build BusyBox

BusyBox provides a single binary with most common Unix utilities. Configure it with a minimal set of applets appropriate for the BE-300:

```bash
make CROSS_COMPILE=mipsel-linux- menuconfig
make CROSS_COMPILE=mipsel-linux-
make CROSS_COMPILE=mipsel-linux- install
```

### 3. Create the Filesystem Image

Create an ext2 filesystem image and populate it with the BusyBox installation:

```bash
# Create a blank image
dd if=/dev/zero of=ramdisk.img bs=1k count=4096
mke2fs -F ramdisk.img

# Mount and populate
mkdir -p /tmp/ramdisk
mount -o loop ramdisk.img /tmp/ramdisk
cp -a _install/* /tmp/ramdisk/

# Create essential directories
mkdir -p /tmp/ramdisk/{dev,proc,sys,tmp,etc}

# Create device nodes
mknod /tmp/ramdisk/dev/console c 5 1
mknod /tmp/ramdisk/dev/null c 1 3

# Add init script
cat > /tmp/ramdisk/etc/init.d/rcS << 'EOF'
#!/bin/sh
mount -t proc proc /proc
EOF
chmod +x /tmp/ramdisk/etc/init.d/rcS

umount /tmp/ramdisk
```

### 4. Compress and Install

```bash
gzip -9 ramdisk.img
cp ramdisk.img.gz arch/mips/ramdisk/ramdisk.gz
```

### 5. Adding a GUI Framework

To include PicoGUI or another graphical environment, cross-compile it for `mipsel-linux` and add the binaries and configuration files to the ramdisk before compressing. The `ramdisk-pgui-full.gz` image provides a reference for the required PicoGUI file layout.

!!! tip "Size Constraints"
    The ramdisk is loaded entirely into RAM. The BE-300 has 16 MB of SDRAM shared between the kernel, ramdisk, and running programs. Keep ramdisk images as small as possible to leave room for application use.
