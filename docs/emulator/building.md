# Building

## Prerequisites

- CMake 3.10 or later
- C11-compatible compiler (GCC or Clang)
- SDL2 development libraries (optional; omit for headless builds)
- Git (for submodule checkout)

## Clone and Build

The GXemul CPU engine is included as a Git submodule. Clone with `--recurse-submodules` to fetch it automatically:

```bash
git clone --recurse-submodules https://github.com/jroark/be300-framebuffer.git
cd be300-framebuffer
```

If you already cloned without submodules:

```bash
git submodule update --init
```

Build with CMake:

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

The resulting `be300` binary is placed in the build directory.

!!! note "macOS"
    On macOS, use `make -j$(sysctl -n hw.ncpu)` instead of `make -j$(nproc)`, or simply `make -j8` (adjust to your core count). SDL2 can be installed via Homebrew: `brew install sdl2`.

## Headless Build

To build without SDL2 (no display window), the build system will automatically detect the absence of SDL2 and compile in headless mode. Serial output still goes to stdout.

## Docker Cross-Tools

A Docker environment provides MIPS cross-compilation toolchains for analyzing BE-300 binaries. This is used for disassembly and reverse engineering, not for building the emulator itself.

```bash
docker compose build mips-dev
docker compose run --rm mips-dev /bin/bash
```

The container includes two toolchains:

| Toolchain | Target | Tools |
|-----------|--------|-------|
| `mipsel-linux-gnu` | Linux ELF | `objdump`, `nm`, `objcopy`, `ar`, `readelf` |
| `mipsel-pe` | WinCE PE | `objdump`, `nm`, `objcopy`, `ar` |

The local source directory is mounted as `/work` inside the container.

!!! warning "Host vs. Container"
    The Docker container is for binary analysis only. Build the emulator on the host. Git operations (commit, push) should also be done from the host, not inside the container.

### Example: Disassemble a WinCE Binary

```bash
# Inside the Docker container
mipsel-pe-objdump -d /work/ce/loader.exe | head -n 50
```

### Example: Disassemble NK.exe

```bash
# Inside the Docker container
mipsel-linux-gnu-objdump -D -b binary -m mips:3000 -EL /work/docs/nk_decompressed.bin | less
```
