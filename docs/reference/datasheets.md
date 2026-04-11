# Datasheets

Technical documentation for the BE-300's primary components. These manuals are the authoritative references for register layouts, timing requirements, and hardware behavior.

## Processor

### NEC VR4131 SoC User Manual

**File:** `Vr4131-um_200203.pdf`

The VR4131 is the main System-on-Chip in the BE-300. This manual covers:

- MIPS32 CPU core (instruction set, CP0 coprocessor, TLB, exceptions)
- Bus Control Unit (BCU)
- Interrupt Control Unit (ICU)
- Clock Management Unit (CMU)
- Power Management Unit (PMU)
- Real-Time Clock (RTC)
- General-purpose I/O Unit (GIU)
- Serial Interface Unit (SIU)
- PCI interface

## Companion Chip

### NEC VRC4173 Companion Chip User Manual

**File:** `U14579EJ2V0UM00.pdf`

The VRC4173 provides additional I/O peripherals connected to the VR4131 via the local bus:

- Interrupt controller (cascaded through VR4131 ICU)
- PS/2 keyboard/mouse interface
- Audio controller
- USB host controller
- Touchscreen (PIU)
- GPIO
- Power management

## Casio Documentation

### BE-300 Driver Design Guidelines

**File:** `DriverDesignGuidlinesforBE-300.pdf`

Casio's guidelines for writing device drivers for the BE-300 platform. Covers the OEM Adaptation Layer (OAL) interface, memory layout expectations, and peripheral access conventions.

### BE-300 Main Board Schematic

**File:** `be300-main-board.pdf`

Board-level schematic of the BE-300 main PCB, showing connections between the VR4131, VRC4173, SDRAM, NAND flash, LCD, and other components. Sourced from jal0's hardware documentation efforts.
