# Touchscreen

The BE-300 has a resistive touchscreen controlled by the VRC4173 PIU
(Pen Input Unit). The current emulator models the PIU behavior needed by the
WinCE touch driver and calibration UI.

## Register Window

| Address | Kseg1 address | Function |
|---------|---------------|----------|
| `0x0A000300` | `0xAA000300` | PIUCNTREG / control and PADSTATE |
| `0x0A000304` | `0xAA000304` | PIUINTREG pending/mask byte pair |
| `0x0A000308` | `0xAA000308` | PIUSIVLREG scan interval |
| `0x0A00030C` | `0xAA00030C` | PIUSTBLREG stabilization timing |
| `0x0A000320`-`0x0A00032C` | `0xAA000320`-`0xAA00032C` | page 0 ADC coordinate buffer |
| `0x0A000350`-`0x0A00035C` | `0xAA000350`-`0xAA00035C` | page 1 ADC coordinate buffer |

The coordinate buffers hold the VRC4173 y+/y-/x-/x+ sample values consumed by
WinCE `touch.dll`.

## Scan Sequencer

The PIU state machine includes:

- WaitPenTouch
- PenDataScan
- IntervalNextScan
- standby/reset behavior

On pen contact, the model raises a pen-change interrupt, samples coordinates
after the configured stabilization/conversion delay, and alternates page 0 and
page 1 valid-buffer interrupts. If both page buffers remain pending, the
data-lost bit is set.

## Interrupt Semantics

PIUINTREG is a pending/mask pair:

- pending bits are set by hardware behavior
- the guest acknowledges with write-one-to-clear
- page 0/page 1 acknowledgements clear the matching valid-buffer state
- the PIU interrupt line is asserted while unmasked pending bits remain

The interrupt routes through the BE-300 cascade:

1. PIU condition changes
2. VRC4173 GIRQ0-9 path asserts
3. GIU line reaches the VR4131 ICU
4. WinCE OAL dispatches touch SYSINTR paths

## Timing

The emulator schedules PIU conversion completion against emulated CP0 Count
cycles. This avoids trace/logging sensitivity: touch behavior should not depend
on how fast the host happens to run the emulator.

The default scan/stabilization values come from real VRC4173 hardware dumps.

## Calibration

After a first boot or factory-reset style state, WinCE presents touch
calibration before the desktop. Calibration expects several stable coordinate
pages for each held pen press, so host click handling deliberately keeps a
minimum guest pen-down dwell time.

Set `BE300_TOUCH_MIN_DWELL_MS=0` for direct host timing, or a larger value to
make short host clicks look like longer stylus presses to the guest.

## Linux Support

The historical Linux4.BE kernels did not implement BE-300 touchscreen support.
Touchscreen work in the current emulator is driven by WinCE calibration and
input behavior.
