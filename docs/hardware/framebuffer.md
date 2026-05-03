# Framebuffer

The BE-300 LCD is modeled as a 240x320 portrait framebuffer mapped through the
VRC4173 companion chip.

## Display Specifications

| Parameter | Value |
|-----------|-------|
| Visible resolution | 240 x 320 pixels |
| Color depth | 16 bits per pixel |
| Main format | RGB565 |
| Framebuffer base | PA `0x0A200000`, kseg1 `0xAA200000` |
| Guest stride | 256 pixels, 512 bytes |

The emulator stores each scanline in a 256-pixel stride and displays the first
240 visible pixels.

## Video Memory Layout

```text
pixel_address = 0xAA200000 + (row * 512) + (col * 2)
```

`row` ranges from `0` to `319`; `col` ranges from `0` to `239`.

## Pixel Format

Pixels are 16-bit little-endian RGB565 values:

```text
Bit: 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
      R  R  R  R  R  G  G  G  G  G  G  B  B  B  B  B
```

| Component | Bits | Range |
|-----------|------|-------|
| Red | 15:11 | 0-31 |
| Green | 10:5 | 0-63 |
| Blue | 4:0 | 0-31 |

Some historical Linux paths use a 5-bit green interpretation. Current emulator
documentation should prefer the source CLI over older removed flags when
describing how to run those kernels.

## WinCE Display Rendering

Windows CE renders the "Initializing..." and "Starting..." screens at runtime.
The strings are not stored as ready-made bitmaps in NK.exe; display/GWE code
fills scratch buffers and blits them to the framebuffer.

The OAL display dispatcher includes modes for clearing the screen and copying
splash/progress buffers into the VRC4173 framebuffer aperture.

## Native UI

The native SDL frontend reads the framebuffer backing store and can show:

- LCD-only mode by default
- BE-300 frame/bezel mode with `--frame`
- scaled output with `--scale 1.0` through `--scale 4.0`
- host-rendered buzzer audio unless `BE300_AUDIO=0` is set

The web and Android frontends reuse the same framebuffer copy API and frame
assets.
