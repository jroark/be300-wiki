# Hibernate and Resume Context

The BE-300's WinCE firmware supports a hibernate/resume mechanism that saves the full CPU state to SDRAM before entering low-power mode. On resume, the saved state is restored to continue execution seamlessly.

## Resume Context (PA 0x2200)

The resume context is a 240-byte structure at physical address `0x2200` that stores the complete CPU state: all general-purpose registers and key CP0 (coprocessor 0) registers.

### GPR Section (Offsets 0x00 -- 0x7C)

The GPR section stores registers in a packed format that **skips `$t0`** (register 8):

| Offset | Register(s) |
|--------|------------|
| `0x00` -- `0x18` | `$at`(1), `$v0`(2), `$v1`(3), `$a0`(4), `$a1`(5), `$a2`(6), `$a3`(7) |
| `0x1C` -- `0x68` | `$t1`(9) through `$gp`(28) |
| `0x6C` | `$sp` (29) |
| `0x70` | `$fp` (30) |
| `0x74` | `$ra` (31) |
| `0x78` | HI |
| `0x7C` | LO |

!!! note
    Register `$t0` (8) is skipped in the save/restore because it is used as a scratch register during the restore sequence itself.

### CP0 Section (Offsets 0x80 -- 0xD0)

The CP0 section stores coprocessor 0 registers in a packed format, skipping registers 7, 8, 15, 19, 21-25, 27, and 31:

| Offset | CP0 Register |
|--------|-------------|
| `0x80` | Index |
| `0x84` | Random |
| `0x88` | EntryLo0 |
| `0x8C` | EntryLo1 |
| `0x90` | Context |
| `0x94` | PageMask |
| `0x98` | Wired |
| `0x9C` | Count |
| `0xA0` | EntryHi |
| `0xA4` | Compare |
| `0xA8` | **Status** |
| `0xAC` | Cause |
| `0xB0` | **EPC** |
| `0xB4` | Config |
| `0xB8` | LLAddr |
| `0xBC` | WatchLo |
| `0xC0` | XContext |
| `0xC4` | ECC |
| `0xC8` | TagLo |
| `0xCC` | TagHi |
| `0xD0` | ErrorEPC |

!!! tip
    The **Status** register is restored **last** (at VA `0x79714` in the restore sequence) to ensure atomicity -- changing Status earlier could enable interrupts or alter exception handling before the rest of the state is consistent.

### ICU Registers (Offsets 0xD4 -- 0xE4)

Following the CP0 section, the ICU (Interrupt Control Unit) register state is saved at offsets `0xD4` through `0xE4`.

### Restore Epilogue

The restore sequence ends at VA `0x797DC`:

```
LW    $t0, 0($sp)
JR    $ra
ADDIU $sp, $sp, 4
```

This pops a value from the stack and returns to whatever address was in `$ra` from the resume context.

## Hibernate State-Save Gating

The function that populates PA `0x2200` (the state-save at `0x76E68-0x76FB4`) is protected by **four checks**, all of which must pass:

| Check | Condition |
|-------|-----------|
| 1 | PA `0x2524` upper 16 bits == `0x3210` (hibernate signature) |
| 2 | VR4131 PMU register `0xC0` bit 4 == 0 |
| 3 | PA `0x2404` != `0x31` |
| 4 | PA `0x254C` bits `0x03` != 0 (hibernate flags) |

!!! warning
    The version check at `0x76CBC` clears PA `0x254C` if PA `0x2400` does not equal `0x03020100`. This prevents check 4 from passing even if `0x254C` was previously set, effectively blocking the hibernate state-save on version mismatch.

## Cold Boot: Resume Context is Not Populated

During a genuine cold boot, **no code populates the resume context at PA 0x2200**. The only writer is `FUN_800792AC`, the scheduler idle state-save function, which only runs after the kernel is fully initialized.

This creates a problem: NK.exe always takes the warm (resume) path due to the [`$t0` clobber bug](nk-cold-boot.md#the-warm-path-bug), and the warm path reads resume_ctx to restore CPU state. On a cold boot, PA `0x2200` contains whatever was in SDRAM at power-on (typically zeros or garbage).

### Emulator Solution

The emulator **seeds PA 0x2200** with cold-start values when it detects that PA `0x24FC` has been written (indicating NK.exe is about to start):

| Field | Offset | Cold-Start Value | Purpose |
|-------|--------|-----------------|---------|
| `$ra` | `0x74` | `0x8007B398` | Cold-start kernel entry point |
| `$sp` | `0x6C` | `0xA0003800` | Initial stack pointer (kseg1) |
| Status | `0xA8` | `0x34400000` | BEV=1, kernel mode, exceptions disabled |

These values cause the warm-path restore to "resume" into the cold-start initialization code at `0x8007B398`, which then builds the kernel state from scratch.

## Key SDRAM Data Structures

The resume context is part of a cluster of boot-related data structures in low SDRAM:

| Address | Size | Contents |
|---------|------|----------|
| PA `0x2200` | 240 bytes | resume_ctx (GPR/CP0 save area) |
| PA `0x2400` | 4 bytes | Version marker (`0x03020100`) |
| PA `0x24FC` | 4 bytes | Next-stage entry address (NK.exe) |
| PA `0x2524` | 4 bytes | Hibernate signature (upper 16 bits `0x3210` = valid) |
| PA `0x254C` | 4 bytes | Hibernate flags (bits `0x03` must be non-zero) |
