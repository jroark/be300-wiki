# NK.exe Cold Boot Flow

This page documents the detailed execution flow of NK.exe during a cold boot, from initial entry through kernel initialization and into the scheduler.

## Entry Point

NK.exe entry is at VA **0x80076B50** (PA 0x00076B50, kseg0). The first instructions:

1. CP0 initialization
2. `JR` to `0xA0076BA0` (kseg1 -- uncached mirror of the same code)
3. Set SP = `0xA0003800`
4. Enable CMU (Clock Management Unit) clocks
5. Call ROM HW init routines

## Early Hardware Init (0x76C60)

After a BCU (Bus Control Unit) revision check, the main hardware initialization path at `0x76C60` performs:

- **Version check** at PA `0x2400` for value `0x03020100`
- **Hibernate signature** check at PA `0x2524` (upper 16 bits == `0x3210`)
- VRC4173 companion chip initialization
- NAND controller initialization
- Cache initialization
- Switch SP to kseg0

## Pre-Init Phase (0xA0079460)

The pre-init continuation calls a series of initialization functions:

- `0x79ADC` -- hardware setup
- `0x7AC50` -- additional init
- `0x79B2C`, `0x79B4C` -- subsystem init
- `J 0x79510` -- PMU/SDRAM configuration loop at `0x79560-0x79598`

## The Warm Path Bug

After the pre-init phase, NK.exe switches to kseg0 via `FUN_800795B4`, which computes address `0x800795D8` and jumps to it. From there:

### check1 (0x80079AC4)

Reads the button register at PA `0x0A00A042`, masks with `0x9E00`, and stores the result in `$v0`. **Critically, it also sets `$t0 = 0xAA00A000`** (always non-zero, a side effect of the LUI used to form the register address).

### check2 (0x8007AFA8)

Reads VRC4173 status registers. Does **not** modify `$t0`.

### Branch Decision (0x795F0)

```
BNE $t0, $zero, 0x79634   # warm path
```

Because `$t0` was set to `0xAA00A000` by check1 and never cleared, this branch is **always taken**. The cold boot initialization code at `0x79DF8` is effectively **dead code**.

!!! warning
    This appears to be a bug in NK.exe. The `$t0` register is clobbered by check1 before it can be tested as a cold/warm discriminator. As a result, NK.exe always takes the warm (resume) path, even on a genuine cold boot. The emulator works around this by seeding the resume context at PA `0x2200` with cold-start values.

## Warm Path: GPR/CP0 Restore (0x79634)

The warm path proceeds through:

1. `JAL 0x78BC0` -- OAL vtable initialization
2. VR41xx hardware setup (timers, ICU, ISR, VRC4173)
3. **CP0 restore** from resume_ctx (PA `0x2200`) at addresses `0x79668-0x79714`
4. **Full GPR restore** from resume_ctx at `0x79730`
5. Epilogue at `0x797DC`: `LW $t0, 0($sp); JR $ra; ADDIU $sp, 4`

The `JR $ra` returns to whatever address was stored in the resume context's RA field. On a seeded cold boot, this is `0x8007B398`.

## Cold-Start Kernel Entry (0x8007B398)

This is the true kernel cold-start initialization, building everything from scratch:

### CP0 Setup

Clears: Cause, EntryHi, Context, EntryLo0, EntryLo1, PageMask, Count.

### Page Table

Zeros the page table at PA `0x1000` (4 KB).

### Section Table

Sets up 64 entries at PA `0x18C0`:

- Default handler: `0x8008BC18`
- Section 9 handler: `0x8008B8E4` (special)

### Stack and Pointers

- SP = `0xA00017E0` (kseg1, no TLB needed)
- Kernel data pointer at PA `0x1AC8` = `0x80000000`
- Kernel data pointer at PA `0x1ACC` = `0x8008B84C`

### TLB Configuration

- PageMask = `0x1800`
- Wired = 2
- Writes fixed TLB entries for kernel address space

### Exception Handlers

Installed via `JAL 0x8007B5F4`:

| Vector Address | Handler Source | Purpose |
|---------------|---------------|---------|
| PA `0x0000` | Code from `0x8008C418` | TLB refill |
| PA `0x0180` | Code from `0x8008B240` | General exception |
| PA `0x0100` | Code from `0x800A8438` | Cache error |

### Kernel Main Init

The critical call:

```
JAL 0x800947C8    # kernel_init(pTOC)
                  # $a0 = 0x80655C54 (pTOC pointer)
```

This function creates processes, loads the **95 XIP modules** referenced by the ROMHDR, and starts the shell.

### Post-Init Calls

After `kernel_init` returns:

1. `JAL 0x80078BC0` -- OAL vtable init
2. `JAL 0x800A5C78` -- display and splash screen setup
3. `JAL 0x800942B4` -- additional kernel init
4. `JAL 0x800964FC` -- final setup
5. Enter the **scheduler**

## NK.exe Memory Layout

```
0x80060000-0x80075FFF  Bootstrap and OAL data
0x80076B50-0x80079xxx  OAL initialization code
0x8007Axxx-0x8007Bxxx  OAL hardware drivers and cold-start
0x80080000-0x800Fffff  Kernel proper (~640KB)
0x800A0000-0x800Bxxxx  OAL callbacks and device drivers
0x80060000-0x80656AC8  Total NK.exe image (6.2MB)
0x80655C54             ROMHDR (pTOC)
0x80660000-0x81000000  RAM (ulRAMStart to ulRAMEnd)
```

## ROMHDR

The ROMHDR structure at pTOC (`0x80655C54`):

| Field | Value |
|-------|-------|
| physfirst | `0x80060000` |
| physlast | `0x80656AC8` |
| nummods | 95 |
| ulRAMStart | `0x80660000` |

## Display Mechanism

The "Initializing..." and "Starting..." splash screens are rendered at runtime:

- OAL display function at `0x80078E10` acts as a blit dispatcher:
    - `a0=10` -- clear screen (fill framebuffer at `0xAA200000`)
    - `a0=0` -- blit splash buffer from VA `0x80061188`
    - `a0=6` -- blit 240x160 pixel buffer from VA `0x80061CD0` to framebuffer
- The buffers are zero-filled in NK.exe and populated at runtime by GWE font rendering

## Kernel Entry Table

Located at VA `0x80074D90`:

| Index | Address | Note |
|-------|---------|------|
| 0 | `0x8008CEA4` | |
| 1 | `0x8009101C` | |
| 2 | `0x80090F34` | |
| 3 | `0x80090F40` | |
| 4 | `0x80090F8C` | |
| 5 | `0x80090FC8` | |
| 6 | `0x80090FF4` | |
| 7 | `0x00000000` | Unused |

Additional entries:

- `0x80074DB0` = `0x80655C54` (pTOC pointer)
- `0x80074DBC` = `"OEM\0"` (ASCII identifier)
