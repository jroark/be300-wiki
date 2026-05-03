# Hibernate and Resume Context

The BE-300 WinCE firmware has a hibernate/resume mechanism that saves CPU and
interrupt-controller state to SDRAM before entering a low-power state. On
resume, NK.exe can restore that state and continue execution.

## Resume Context At PA 0x2200

The resume context is a low-SDRAM structure beginning at physical address
`0x2200`. It stores general-purpose registers, selected CP0 registers, and ICU
state used by the resume path.

### GPR Section

The GPR section stores most MIPS registers and skips `$t0`, which is used as a
scratch register during restore.

| Offset | Register(s) |
|--------|-------------|
| `0x00`-`0x18` | `$at`, `$v0`, `$v1`, `$a0`-`$a3` |
| `0x1C`-`0x68` | `$t1` through `$gp` |
| `0x6C` | `$sp` |
| `0x70` | `$fp` |
| `0x74` | `$ra` |
| `0x78` | HI |
| `0x7C` | LO |

### CP0 Section

The CP0 section begins at offset `0x80` and includes the registers needed to
restore MMU and exception state.

| Offset | CP0 register |
|--------|--------------|
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
| `0xA8` | Status |
| `0xAC` | Cause |
| `0xB0` | EPC |
| `0xB4` | Config |
| `0xB8` | LLAddr |
| `0xBC` | WatchLo |
| `0xC0` | XContext |
| `0xC4` | ECC |
| `0xC8` | TagLo |
| `0xCC` | TagHi |
| `0xD0` | ErrorEPC |

Status is restored late so interrupts and exception-mode changes do not take
effect before the rest of the state is coherent.

### ICU State

ICU state follows the CP0 section. This lets the resume path recover interrupt
pending/mask state alongside CPU state.

## Hibernate State-Save Gating

The hibernate state-save path is gated by low-memory signatures, PMU state, and
hibernate flags. These checks prevent arbitrary SDRAM contents from being used
as a resume image.

Important low-memory locations:

| Address | Contents |
|---------|----------|
| PA `0x2200` | Resume context |
| PA `0x2400` | Version marker |
| PA `0x24FC` | Next-stage entry mailbox |
| PA `0x2524` | Hibernate signature |
| PA `0x254C` | Hibernate flags |

## Cold Boot Caveat

During a genuine cold boot, the resume context is not a precondition for
starting WinCE. Current emulator cold boots run through ROM, SPL, NK.exe, and
first-boot UI without seeding PA `0x2200`.

Ground-truth captures can still show PA `0x2200` populated after the system has
booted, because the scheduler/idle hibernate path can write it during normal
runtime. Treat resume-context dumps as phase-sensitive data, not reset-time
state.
