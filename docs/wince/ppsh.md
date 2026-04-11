# PPSH Debug Interface

**PPSH** (Parallel Port Shell) is a Casio debug interface built into the BE-300's WinCE firmware. It is not a companion MCU or external hardware -- it is a software debug shell that communicates through two I/O registers in the BE-300's address space.

## Registers

| Register | Physical Address | Description |
|----------|-----------------|-------------|
| Data | PA `0x0C000120` | Data read/write |
| Status/Command | PA `0x0C000520` | Status flags and command port |

The two registers are at offsets `0x000` and `0x400` from the base address `0x0C000120`.

## Detection Protocol

NK.exe probes for PPSH presence using a **two-level detection** scheme:

### Level 1: Hardware Test

A basic read of the status register at offset `0x004` from a test base address. This is not wrapped in an exception handler -- a bus error here would be fatal. On real hardware without a PPSH controller attached, this read returns zero.

### Level 2: Probe Command

NK.exe writes command `0x3330` to the status register and checks:

```
if ((status & 0x2320) == 0x2320) → PPSH present
```

This probe **is** wrapped in a `__try` block, so a bus error is handled gracefully.

!!! note
    Only accesses to offsets `0x000` and `0x400` from the PPSH base can trigger bus errors. The Level 1 test at offset `0x004` does not fault.

## Boot Path Impact

The PPSH detection result determines the entire boot path:

| Probe Result | Boot Behavior |
|-------------|---------------|
| `0x0000` (no controller) | Normal GUI boot -- desktop, touch calibration |
| `0x2320` (PPSH present) | Debug shell -- WinCE routes all console I/O to PPSH, bypasses GUI |

When PPSH is detected, WinCE boots to a `Windows CE>` command prompt instead of the graphical shell.

## Protocol

PPSH uses a framed message protocol:

| Field | Value |
|-------|-------|
| Sync word | `0xAA5555AA` |
| Trailer | `0x5AA50A1A` |

### Message Types

| Type | Description |
|------|-------------|
| `0x02` | Process load notification |
| `0x05` | Console write (text output) |

## Status Bit Semantics

| Bit Mask | Meaning |
|----------|---------|
| `0x2000` | Present |
| `0x0200` | Active |
| `0x0100` | Init |
| `0x0020` | Ready |
| `0x0002` | Busy |
| `0x1000` | Data available |

## NK.exe PPSH Functions

All PPSH functions are called via function pointers stored at VA `0x800B8050-0x800B8078`:

| Address | Function | Description |
|---------|----------|-------------|
| `0x8007846C` | ppsh_read_response | Polls bit `0x1000` in status register, reads upper byte of data register |
| `0x800784F4` | ppsh_send_command | Writes command byte to data register, dispatches via handshake sequence |

### Command Sequence

The send operation follows this protocol:

1. Write data byte to data register
2. Send `0x1100` (dispatch)
3. Poll for bit `0x0002` (busy) to clear
4. Send `0x9100` (acknowledge)
5. Send `0x9900` (complete)
6. Send `0x3330` (re-probe / status refresh)

!!! warning
    With PPSH enabled, the boot process takes significantly longer due to the polling overhead in the PPSH command protocol. Each poll cycle through the scheduler's time-slicing adds latency, and the full PPSH boot can take 10+ minutes of wall time in emulation.
