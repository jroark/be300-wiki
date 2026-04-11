# Register Reference

This page documents the VR4131 on-chip peripheral registers as observed on real BE-300 hardware, with comparison to earlier NEC VR-series chips (VR4102, VR4181) where the register layout differs.

## VR4131 Register Comparison

The VR41xx family shares a common peripheral architecture, but register offsets and available peripherals vary by chip revision. The BE-300 uses the VR4131; earlier MIPS handhelds used the VR4102 or VR4181.

### Bus Control Unit (BCU)

Base: `0xAF000000`

| Offset | VR4131 | VR4102 | VR4181 | Register |
|---|---|---|---|---|
| 0x000 | Y | Y | Y | BCUCNTREG1 |
| 0x002 | -- | Y | -- | BCUCNTREG2 |
| 0x004 | Y | -- | -- | BCUCNTREG2 (4131) |
| 0x00C | Y | Y | Y | ROMSIZEREG |
| 0x010 | Y | -- | -- | BCUSPEEDREG |
| 0x012 | -- | Y | Y | BCUSPEEDREG |
| 0x014 | Y | -- | -- | BCUERRSTREG |
| 0x01A | Y | -- | -- | BCURFCNTREG |
| 0x01C | Y | -- | -- | REVIDREG |
| 0x020 | Y | -- | -- | BCURFCOUNTREG |
| 0x024 | Y | -- | -- | CLKSPEEDREG |

### Clock Mask Unit (CMU)

Base: `0xAF000060`

| Offset | VR4131 | VR4102 | VR4181 | Register |
|---|---|---|---|---|
| 0x060 | Y | Y | Y | CMUCLKMSK |
| 0x062 | Y | -- | -- | CMUCLKMSK2 |

### Interrupt Control Unit (ICU)

Base: `0xAF000080`

| Offset | Register | Description |
|---|---|---|
| 0x080 | SYSINT1REG | System interrupt source 1 |
| 0x082 | -- | Reserved |
| 0x084 | GIUINTLREG | GIU interrupt (lower, via 0xAF000088) |
| 0x088 | MSYSINT1REG | System interrupt 1 mask |
| 0x08C | MGIUINTLREG | GIU interrupt mask (lower) |
| 0x08E | MGIUINTHREG | GIU interrupt mask (upper) |
| 0x090 | NMIREG | NMI register |
| 0x092 | SOFTINTREG | Software interrupt |
| 0x09C | SYSINT2REG | System interrupt source 2 |
| 0x09E | GIUINTHREG | GIU interrupt (upper) |
| 0x0A0 | MSYSINT2REG | System interrupt 2 mask |

### Power Management Unit (PMU)

Base: `0xAF0000C0`

| Offset | Register | Description |
|---|---|---|
| 0x0C0 | PMUCNTREG | PMU control |
| 0x0C2 | PMUCNT2REG | PMU control 2 |

### Real-Time Clock (RTC)

Base: `0xAF000100`

| Offset | Register | Description |
|---|---|---|
| 0x100 | ETIMELREG | Elapsed time (low 16 bits) |
| 0x102 | ETIMEMREG | Elapsed time (mid 16 bits) |
| 0x104 | ETIMEHREG | Elapsed time (high 16 bits) |
| 0x106 | ECMPLREG | Elapsed time compare (low) |
| 0x108 | ECMPMREG | Elapsed time compare (mid) |
| 0x10A | ECMPHREG | Elapsed time compare (high) |
| 0x10C | RTCL1LREG | RTC long 1 counter (low) |
| 0x10E | RTCL1HREG | RTC long 1 counter (high) |
| 0x110 | RTCL1CNTLREG | RTC long 1 compare (low) |
| 0x112 | RTCL1CNTHREG | RTC long 1 compare (high) |
| 0x114 | RTCL2LREG | RTC long 2 counter (low) |
| 0x116 | RTCL2HREG | RTC long 2 counter (high) |
| 0x118 | RTCL2CNTLREG | RTC long 2 compare (low) |
| 0x11A | RTCL2CNTHREG | RTC long 2 compare (high) |
| 0x11E | RTCINTREG | RTC interrupt (write-1-to-clear) |

### General-purpose I/O Unit (GIU)

Base: `0xAF000140`

| Offset | Register | Description |
|---|---|---|
| 0x140 | GIUIOSELL | I/O select (lower) |
| 0x142 | GIUIOSELH | I/O select (upper) |
| 0x144 | GIUPIODL | Port I/O data (lower) |
| 0x146 | GIUPIODH | Port I/O data (upper) |
| 0x148 | GIUINTSTATL | Interrupt status (lower) |
| 0x14A | GIUINTSTATH | Interrupt status (upper) |
| 0x14C | GIUINTENL | Interrupt enable (lower) |
| 0x14E | GIUINTENH | Interrupt enable (upper) |
| 0x150 | GIUINTTYPL | Interrupt type (lower) -- edge/level |
| 0x152 | GIUINTTYPH | Interrupt type (upper) |
| 0x154 | GIUINTALSELL | Interrupt alt select (lower) |
| 0x156 | GIUINTALSELH | Interrupt alt select (upper) |
| 0x158 | GIUINTHTSELL | Interrupt high/low select (lower) |
| 0x15A | GIUINTHTSELH | Interrupt high/low select (upper) |
| 0x164 | GIUPODAT | GIU port output data |
| 0x15C | GIUODERL | GIU open-drain enable (lower) |
| 0x15E | GIUODERH | GIU open-drain enable (upper) |

### SDRAM Controller (SDRAMU)

Base: `0xAF000400`

| Offset | Register | Description |
|---|---|---|
| 0x400 | SDRAMMODEREG | Mode register |
| 0x402 | SDRAMCNTREG | Control register |
| 0x404 | SDRAMACCREG | Access timing |
| 0x406 | SDRAMREFREG | Refresh interval |

### PCI Interface

Base: `0xAF000C00`

| Offset | Register | Description |
|---|---|---|
| 0xC00 | PCIMMAW1REG | PCI master memory address window 1 |
| 0xC04 | PCIMMAW2REG | PCI master memory address window 2 |
| 0xC08 | PCITAW1REG | PCI target address window 1 |
| 0xC0C | PCITAW2REG | PCI target address window 2 |
| 0xC10 | PCIMIOAWREG | PCI master I/O address window |
| 0xC18 | PCICONFDREG | PCI configuration data |
| 0xC1C | PCICONFAREG | PCI configuration address |
| 0xC20 | PCIMAILREG | PCI mailbox |
| 0xC40 | BUABORTREG | Bus abort status |
| 0xC5C | PCIEXABORTREG | PCI exception abort |
| 0xC60 | PCIRECVEXABORTREG | PCI received exception abort |
| 0xC68 | PCIENREG | PCI enable |
| 0xC6C | PCICLKMASKREG | PCI clock mask |
| 0xC80 | PCITRDYREG | PCI TRDY |

## PCI Configuration Space

Base: `0xAF000D00`

| Offset | Register | Description |
|---|---|---|
| 0xD00 | Vendor/Device ID | 0x1033 (NEC) / device |
| 0xD04 | Command/Status | PCI command register |
| 0xD08 | Revision/Class | Class code |
| 0xD10 | BAR0 | Base address register 0 |
| 0xD14 | BAR1 | Base address register 1 |
| 0xD3C | Int Line/Pin | Interrupt routing |

## VR4131 Hardware Register Dump (Real Hardware)

!!! tip "Ground Truth"
    Register values documented here were captured from a real BE-300 device. See [Ground Truth Data](../reference/ground-truth.md) for methodology and full dumps.

Key observed values from a running WinCE system:

| Register | Address | Value | Notes |
|---|---|---|---|
| BCUCNTREG1 | 0xAF000000 | 0x0033 | Bus control |
| REVIDREG | 0xAF00001C | 0x00000C81 | Chip revision = VR4131 |
| CMUCLKMSK | 0xAF000060 | 0x003F | All clocks enabled |
| SYSINT1REG | 0xAF000080 | varies | Active interrupt sources |
| RTCINTREG | 0xAF00011E | varies | RTC interrupt status |
