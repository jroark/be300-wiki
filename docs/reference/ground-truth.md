# Ground Truth Data

Hardware register dumps and memory snapshots captured from a real Casio BE-300 running stock Windows CE 3.0. These serve as the definitive reference for validating emulator behavior.

## Dump Files

### hw_dump_vr4131.txt

VR4131 SoC register dumps organized by functional block:

- **BCU** -- Bus Control Unit configuration and chip select registers
- **ICU** -- Interrupt Control Unit status and mask registers
- **PMU/RTC** -- Power Management Unit state and Real-Time Clock registers
- **GIU** -- General-purpose I/O pin state and configuration
- **PCI** -- PCI controller registers

### hw_dump_vrc4173.txt

VRC4173 companion chip register dumps covering all I/O subsystems accessible from the host bus at base address `0x0A000000`.

### hw_dump_memory.txt

Memory region dumps:

- **SDRAM** -- Key data structure contents in the first 64KB of RAM
- **Exception vectors** -- Code installed at PA `0x0000` (TLB refill), PA `0x0100` (cache error), PA `0x0180` (general exception)
- **Boot parameters** -- Version markers, hibernate signatures, and resume context at PA `0x2200`-`0x2700`
- **ROM** -- Verification dump of the 16KB boot ROM at PA `0x1FC00000`

### hw_dump_tlb.txt

Complete TLB state snapshots showing all 32 entries with:

- Virtual page number (EntryHi)
- Physical frame numbers (EntryLo0, EntryLo1)
- Page mask (page size)
- ASID and global flags

### hw_dump_diffs.txt

Differential memory dumps and focused probes:

- Memory region diffs between capture phases
- Targeted probes of registers that change during boot
- Region activity summaries

## Key Reference Values

These values from the real hardware are used to validate emulator register initialization:

| Register / Location | Value | Notes |
|---------------------|-------|-------|
| VRC4173 Board ID | `0x7100` | Read from VRC4173 identification register |
| CP0 Status | `0x00008401` | Post-boot kernel mode, BEV=0, IE=1 |
| CP0 Config | `0x10135923` | Cache configuration, kseg0 coherency |
| CP0 PRId | `0x00000C81` | VR4131 processor identification |

## Phase-Sensitive Regions

Some hardware registers and memory locations change value depending on when they are read during the boot process. These require careful timing in the emulator:

| Address | Description | Sensitivity |
|---------|-------------|-------------|
| `0x0A000C38` | NAND controller status | Changes during flash operations |
| `0x0F000100`+ | VR4131 timer registers | Continuously incrementing |
| PA `0x2200` | Resume context | Written by scheduler idle path, not populated during cold boot init |

## Capture Methodology

All dumps were captured from a real BE-300 device using the [BEDiag](../emulator/bediag.md) diagnostic stream driver. The device was running its factory-installed Windows CE 3.0 image. Captures were taken at multiple points after boot to observe state evolution.

!!! note "Cold boot vs. running state"
    The register dumps reflect the system state after WinCE has fully booted and the desktop is active. Early boot state (ROM execution, SPL, NK.exe initialization) is not directly captured -- it is reconstructed through reverse engineering of the boot code.
