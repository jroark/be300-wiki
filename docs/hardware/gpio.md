# GPIO and Buttons

The BE-300 has hardware buttons managed through the VR4131 GIU (General-purpose I/O Unit) and a button register on the VRC4173 companion chip.

## GIU Registers

The VR4131 GIU is at PA `0x0F000140` (kseg1: `0xAF000140`):

| Offset | Address | Register | Description |
|---|---|---|---|
| 0x140 | 0xAF000140 | GIUIOSELL | I/O pin select (lower 16 pins) |
| 0x142 | 0xAF000142 | GIUIOSELH | I/O pin select (upper 16 pins) |
| 0x144 | 0xAF000144 | GIUPIODL | Port I/O data (lower) |
| 0x146 | 0xAF000146 | GIUPIODH | Port I/O data (upper) |
| 0x148 | 0xAF000148 | GIUINTSTATL | Interrupt status (lower) |
| 0x14C | 0xAF00014C | GIUINTENL | Interrupt enable (lower) |
| 0x150 | 0xAF000150 | GIUINTTYPL | Interrupt type (lower) -- edge/level |
| 0x158 | 0xAF000158 | GIUINTHTSELL | Interrupt polarity (lower) |

## Button Register

The VRC4173 button/key register is at `0x0A00A042` (kseg1: `0xAA00A042`). NK.exe reads this register with a mask of `0x9E00` to determine button state.

## Hardware Buttons

The BE-300 has the following physical buttons:

| Button | Function |
|---|---|
| OK | Confirm / Enter |
| ESC | Cancel / Back |
| Power | Power on/off |
| Up | Navigation up |
| Down | Navigation down |
| Left | Navigation left |
| Right | Navigation right |
| Rocket | Application launch |

## Keyboard Interrupt

Button presses generate a keyboard interrupt mapped to SYSINTR_KEYBOARD (0x10) in Windows CE.

## GIU Interrupt Lines

The GIU provides interrupt lines that cascade into the VR4131 ICU via SYSINT1REG bit 8. Key GIU interrupt assignments:

| GIU Line | GIUINTLREG Bit | Source |
|---|---|---|
| GIRQ0 | 0 | CompactFlash / VRC4173 sub-cascade |
| GIRQ1 | 1 | VRC4173 peripheral |
| GIRQ7 | 7 | VRC4173 peripheral |
| GIRQ9 | 9 | Touchscreen pen-down |
| GIRQA | 0xA | VRC4173 peripheral |
| GIRQB | 0xB | VRC4173 peripheral |

See [Interrupts](interrupts.md) for the full interrupt cascade documentation.

## NK.exe Cold Boot Check

During cold boot, NK.exe reads the button register at `0xAA00A042` as part of check1 (VA `0x80079AC4`). The code masks the value with `0x9E00` and stores the result in `$v0`, but also unconditionally loads `$t0 = 0xAA00A000` (the LUI used to form the register address). Since `$t0` is always non-zero, the subsequent BNE always takes the warm-path branch regardless of button state.
