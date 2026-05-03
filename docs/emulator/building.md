# Building

## Prerequisites

- CMake 3.10 or later
- C11-compatible compiler
- SDL2 development libraries for the native framebuffer window and buzzer audio
- libpng if SDL frame PNG support is desired
- Git submodules initialized for the GXemul CPU engine
- `gtimeout` or GNU `timeout` for bounded boot smoke tests

## Clone And Submodules

```bash
git clone --recurse-submodules https://github.com/jroark/be300-framebuffer.git
cd be300-framebuffer
```

If the repository was cloned without submodules:

```bash
git submodule update --init
```

## Native CLI Build

```bash
mkdir -p build-host
cd build-host
cmake ..
make -j$(nproc)
```

On macOS, use a fixed job count or `$(sysctl -n hw.ncpu)` instead of
`$(nproc)`:

```bash
make -j$(sysctl -n hw.ncpu)
```

The native executable is written to `build-host/be300`.

!!! note "Apple Silicon"
    The CMake configuration defaults to `arm64` on Apple Silicon hardware when
    no `CMAKE_OSX_ARCHITECTURES` value is supplied. Homebrew SDL2 and libpng
    from `/opt/homebrew` are detected when available.

## Headless Build

SDL2 is optional for the native CLI. If SDL2 is not found, CMake builds a
headless binary. Serial output and diagnostics still work, but no host display
or audio window is opened.

## WebAssembly Build

The web frontend reuses the same emulator core and adds Emscripten glue in
`web/main_web.c`.

```bash
./tools/build_web.sh
```

The generated browser bundle is written to `build-web/web/`. See
[Web And Android Frontends](frontends.md) for local serving and runtime notes.

## Android Build

The Android app embeds the emulator core through JNI and renders the framebuffer
in Java/Kotlin UI code:

```bash
cd android
./gradlew :app:assembleDebug
```

The debug APK is written to `android/app/build/outputs/apk/debug/app-debug.apk`.

## Docker Cross-Tools

The Docker `mips-dev` environment is for MIPS and WinCE binary analysis, not
for building the native emulator:

```bash
docker compose build mips-dev
docker compose run --rm mips-dev /bin/bash
```

The source tree is mounted at `/work` inside the container.

| Toolchain | Target | Examples |
|-----------|--------|----------|
| `mipsel-linux-gnu` | Linux ELF and raw MIPS disassembly | `objdump`, `nm`, `readelf` |
| `mipsel-pe` | WinCE PE binaries | `objdump`, `nm`, `objcopy` |

Example:

```bash
mipsel-linux-gnu-objdump -D -b binary -m mips:3000 -EL /work/docs/nk_decompressed.bin
```
