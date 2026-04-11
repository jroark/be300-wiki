# Toolchain

## Overview

Building Linux kernels and applications for the BE-300 requires a MIPS little-endian cross-compilation toolchain targeting `mipsel-linux`.

## Pre-Built Toolchains

### Windows

jal0 provided pre-built Windows cross-tools:

- **CEK.zip** --- cross-compilation environment for Windows
- **Binutils** --- pre-built MIPS binutils for Windows

### Linux (TimeSys BSP)

TimeSys provided a Board Support Package (BSP) that included a pre-built cross-toolchain for the VR4131 target.

## Building from Source

For a complete Linux-hosted cross-toolchain, build the following components in order.

### Step 1: Cross-Binutils (2.11.92)

```bash
tar xzf binutils-2.11.92.tar.gz
cd binutils-2.11.92
./configure --target=mipsel-linux --prefix=/usr/local/cross
make
make install
export PATH=/usr/local/cross/bin:$PATH
```

### Step 2: Bootstrap C Cross-Compiler (GCC 3.0.3)

Build a minimal C-only compiler without shared library support:

```bash
tar xzf gcc-3.0.3.tar.gz
cd gcc-3.0.3
./configure \
    --target=mipsel-linux \
    --prefix=/usr/local/cross \
    --enable-languages=c \
    --disable-shared \
    --disable-threads
make
make install
```

### Step 3: Cross-Build glibc (2.2.4)

```bash
tar xzf glibc-2.2.4.tar.gz
cd glibc-2.2.4
./configure \
    --build=i686-linux \
    --host=mipsel-linux \
    --prefix=/usr/local/cross/mipsel-linux \
    --enable-add-ons=linuxthreads
make
make install
```

### Step 4: Fix libc.so

Edit `/usr/local/cross/mipsel-linux/lib/libc.so` to point to the cross-built library paths rather than the host paths. This is a text file containing linker script directives --- update the absolute paths to reference the cross-compiled libraries.

### Step 5: Final C/C++ Compiler

Rebuild GCC with full shared library and C++ support:

```bash
cd gcc-3.0.3
make distclean
./configure \
    --target=mipsel-linux \
    --prefix=/usr/local/cross \
    --enable-languages=c,c++ \
    --enable-shared
make
make install
```

The toolchain is now installed at `/usr/local/cross/` with the `mipsel-linux-` prefix (e.g., `mipsel-linux-gcc`, `mipsel-linux-ld`).

## Modern Alternative

The BE-300 emulator project includes a Docker container with a modern `mipsel-linux-gnu` toolchain suitable for analyzing and building MIPS little-endian ELF binaries:

```bash
docker compose build mips-dev
docker compose run --rm mips-dev /bin/bash
```

This provides tools such as `mipsel-linux-gnu-gcc`, `mipsel-linux-gnu-objdump`, and `mipsel-linux-gnu-nm`. The container also includes a `mipsel-pe` toolchain for analyzing Windows CE PE binaries.

!!! note "Kernel Compatibility"
    The original Linux4.BE kernels were built with GCC 3.0.x. Using a significantly newer compiler may require kernel source adjustments. The 2.4.18 kernel source expects GCC 3.x-era behavior.
