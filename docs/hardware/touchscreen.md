# Touchscreen

The BE-300 has a resistive touchscreen overlay on the LCD panel, controlled by the VRC4173 companion chip PIU (Pen Input Unit).

## Registers

| Address | Kseg1 Address | Function |
|---|---|---|
| 0x0A000000 | 0xAA000000 | PIU base |
| 0x0A000301 | 0xAA000301 | Touchpanel register |
| 0x0A000304 | 0xAA000304 | Pen-down detection |
| 0x0A00031C | 0xAA00031C | Touchpanel register |

## Pen-Down Detection

The pen-down register at `0xAA000304` indicates whether the stylus is touching the screen. When a touch is detected, the PIU asserts an interrupt through the GIU cascade.

## Interrupt Path

The touchscreen interrupt follows this path through the interrupt cascade:

1. Stylus contacts the touchscreen
2. VRC4173 PIU detects pen-down
3. GIU line 9 asserts
4. GIUINTLREG (0xAF000088) bit 9 is set
5. SYSINT1REG (0xAF000080) bit 8 (GIUINTR) asserts
6. VR4131 ICU dispatches to GIU handler
7. Handler reads pen-down register at `0xAA000304`
8. Returns SYSINTR_TOUCH (0x11) for initial pen-down, or SYSINTR_TOUCH_CHANGED (0x18) for movement

## WinCE Touch Calibration

After a cold boot, the first user interaction in Windows CE is the touch calibration screen. The OS presents crosshair targets that the user must tap with the stylus to establish the mapping between touch coordinates and screen pixels.

This calibration screen appears after the "Starting..." splash screen and before the WinCE desktop loads.

## Linux Support

The Linux4.BE project did not implement touchscreen support. The Linux kernels use only the framebuffer console and serial console for I/O.
