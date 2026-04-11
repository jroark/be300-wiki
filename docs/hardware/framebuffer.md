# Framebuffer

The BE-300 display is a 320x240 color STN LCD panel driven by an EPSON L50753 controller, with video memory mapped through the VRC4173 companion chip.

## Display Specifications

| Parameter | Value |
|---|---|
| Resolution | 320 x 240 pixels |
| Color Depth | 16 bits per pixel |
| Color Format | RGB 5:6:5 (default) or RGB 5:5:5 |
| LCD Controller | EPSON L50753 |
| Panel Type | Color STN |

## Video Memory Layout

| Parameter | Value |
|---|---|
| Base Address | PA 0x0A200000 (kseg1: 0xAA200000) |
| Pixel Size | 2 bytes (16 bits) |
| Line Stride | 512 bytes |
| Active Pixels per Line | 320 (640 bytes) |
| Padding per Line | 192 bytes (unused) |
| Total Framebuffer | 240 x 512 = 122,880 bytes (~120 KB) |

### Pixel Address Formula

```
pixel_address = 0xAA200000 + (row * 512) + (col * 2)
```

Where `row` ranges from 0 to 239 and `col` ranges from 0 to 319.

## Pixel Format

Each pixel is stored as a 16-bit little-endian value:

### RGB 5:6:5 (Default)

```
Bit: 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
      R  R  R  R  R  G  G  G  G  G  G  B  B  B  B  B
```

| Component | Bits | Range |
|---|---|---|
| Red | 15:11 | 0-31 |
| Green | 10:5 | 0-63 |
| Blue | 4:0 | 0-31 |

### RGB 5:5:5 (Alternate)

Some display modes use 5 bits per channel with bit 15 unused:

```
Bit: 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
      X  R  R  R  R  R  G  G  G  G  G  B  B  B  B  B
```

!!! tip "5-Bit Green Mode"
    The Linux 2.6 kernel uses RGB 5:5:5 format. When running `vmlinux-2.6` in the emulator, use the `--sfb-5bit-green` flag for correct colors.

## WinCE Display Rendering

Windows CE renders the "Starting..." and "Initializing..." splash screens at runtime using the GWE (Graphics, Windowing, and Events) subsystem. These strings are not stored as bitmaps or text in NK.exe -- they are rendered through font rendering into framebuffer scratch areas.

### OAL Display Function (0x80078E10)

The OAL display blit dispatcher accepts a mode parameter in `$a0`:

| Mode ($a0) | Action |
|---|---|
| 0 | Blit splash buffer from VA 0x80061188 to framebuffer |
| 6 | Blit 240x160 pixel buffer from VA 0x80061CD0 (progress bar) |
| 10 | Clear screen (fill framebuffer with solid color) |

The splash buffers at 0x80061188 and 0x80061CD0 are zero-filled in the NK.exe binary. GWE populates them at runtime after the display driver is initialized.

## Framebuffer in Linux

The Linux kernels use a simple framebuffer driver that maps the video memory directly. The framebuffer console (`console=tty0`) renders text using a built-in font. PicoGUI uses the framebuffer for its windowing system.
