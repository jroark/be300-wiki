# Ground Truth Data

Ground-truth files are hardware register dumps, memory snapshots, boot-ROM
captures, and diagnostic outputs from real BE-300 hardware. They are used to
validate emulator behavior, but many values are phase-sensitive and should not
be copied blindly into reset-time state.

## Dump Files

| File | Contents |
|------|----------|
| `hw_dump_vr4131.txt` | VR4131 BCU, ICU, PMU/RTC, GIU, and related SoC registers |
| `hw_dump_vrc4173.txt` | VRC4173 companion-chip register windows |
| `hw_dump_memory.txt` | SDRAM structures, exception vectors, ROM verification data |
| `hw_dump_tlb.txt` | TLB state snapshots |
| `hw_dump_diffs.txt` | Differential memory/register probes across capture phases |
| `BE300BootROM_v1.txt` / `be300_boot_rom.*` | Masked boot ROM disassembly and analysis inputs |

## Key Reference Values

| Register / Location | Value | Notes |
|---------------------|-------|-------|
| VRC4173 Board ID | `0x7100` | Companion identification |
| CP0 Status | `0x00008401` | Post-boot running-state example |
| CP0 Config | `0x10135923` | Cache configuration sample |
| CP0 PRId | `0x00000C81` | VR4131 processor identification observed on hardware |

## Phase-Sensitive Data

Some captured values describe a running WinCE system, not reset state.

| Region | Caveat |
|--------|--------|
| PA `0x2200` resume context | Written by hibernate/scheduler paths after boot; not required for reset-time cold boot |
| VRC4173 CF/PCMCIA words | Inserted-card captures are not no-card defaults |
| RTC/elapsed counters | Change continuously and depend on guest date/time state |
| PIU/touch registers | Depend on scan state, pending page buffers, and acknowledgements |
| ICU pending/mask registers | Depend on interrupt timing and guest acknowledgement order |

## Capture Methodology

Most dumps were captured from a real BE-300 running stock Windows CE 3.0 using
the [BEDiag](../emulator/bediag.md) diagnostic stream driver. BEDiag captures
multiple post-boot snapshots so the emulator can distinguish stable hardware
identity from runtime state.

!!! note "Use with source"
    Treat the maintained emulator source and `docs/CURRENT_STATUS.md` in
    `../be300-framebuffer` as the current interpretation layer for these raw
    captures. Raw dump values remain evidence, but source comments often record
    whether a value is reset-time, inserted-card, no-media, or post-boot state.
