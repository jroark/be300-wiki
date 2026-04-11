# Memory Map

This page documents the complete physical address map of the BE-300, including SDRAM, peripheral I/O, boot ROM, and key data structures.

## Physical Address Map

| Address Range | Size | Description |
|---|---|---|
| 0x00000000 - 0x00FFFFFF | 16 MB | SDRAM |
| 0x0A000000 - 0x0A00FFFF | 64 KB | VRC4173 companion chip I/O |
| 0x0A200000 - 0x0A227FFF | ~160 KB | Framebuffer (video memory) |
| 0x0C000120 - 0x0C000620 | -- | PPSH debug interface |
| 0x0F000000 - 0x0F000FFF | 4 KB | VR4131 on-chip peripheral I/O |
| 0x1FC00000 - 0x1FC03FFF | 16 KB | Masked Boot ROM |

!!! note "Kseg1 Mapping"
    Hardware registers are typically accessed through kseg1 (uncached, unmapped) by adding 0xA0000000 to the physical address. For example, PA 0x0F000080 (ICU) is accessed at VA 0xAF000080.

## VR4131 On-Chip Peripheral Map

Base physical address: `0x0F000000` (kseg1: `0xAF000000`)

| Offset | Kseg1 Address | Peripheral | Description |
|---|---|---|---|
| 0x000 | 0xAF000000 | BCU | Bus Control Unit / VRIP |
| 0x060 | 0xAF000060 | CMU | Clock Mask Unit |
| 0x080 | 0xAF000080 | ICU | Interrupt Control Unit |
| 0x0C0 | 0xAF0000C0 | PMU | Power Management Unit |
| 0x100 | 0xAF000100 | RTC | Real-Time Clock |
| 0x140 | 0xAF000140 | GIU | General-purpose I/O Unit |
| 0x400 | 0xAF000400 | SDRAMU | SDRAM Controller |
| 0x800 | 0xAF000800 | SIU | Serial Interface Unit |
| 0x820 | 0xAF000820 | DSIU | Debug Serial Interface Unit |
| 0xC00 | 0xAF000C00 | PCI | PCI Interface |
| 0xD00 | 0xAF000D00 | PCICONF | PCI Configuration |

## VRC4173 Companion Chip Map

Base physical address: `0x0A000000` (kseg1: `0xAA000000`)

| Offset | Kseg1 Address | Peripheral | Description |
|---|---|---|---|
| 0x000 | 0xAA000000 | PIU/CAK/GPIO | Peripheral interface base |
| 0x004 | 0xAA000004 | GIRQ0 | Sub-cascade interrupt status |
| 0x060 | 0xAA000060 | SYSINT1REG | System interrupt 1 (read-only) |
| 0x301 | 0xAA000301 | Touch | Touchpanel register |
| 0x304 | 0xAA000304 | Touch | Pen-down detection |
| 0x31C | 0xAA00031C | Touch | Touchpanel register |
| 0x980 | 0xAA000980 | Buzzer | BlgReg_BASE (size 0x68) |
| 0x1000 | 0xAA001000 | CF | CompactFlash status |
| 0x1120 | 0xAA001120 | IRQ | Interrupt register |
| 0x112C | 0xAA00112C | IRQ | Interrupt register |
| 0x1128 | 0xAA001128 | CmmReg | CmmReg_BASE |
| 0x1B20 | 0xAA001B20 | IRQ | Interrupt register |
| 0x7800 | 0xAA007800 | ScCmcu | Sub-controller |
| 0x8000 | 0xAA008000 | Vic | Vic / CommMode |
| 0x8680 | 0xAA008680 | SIU | Serial UART (16550-compatible) |
| 0xA000 | 0xAA00A000 | KjCMU | Clock Management Unit |
| 0xA042 | 0xAA00A042 | Buttons | Button/key register |

### VRC4173 NAND Controller Registers

| Offset | Kseg1 Address | Function |
|---|---|---|
| 0xB000 | 0xAA00B000 | Stream data read buffer |
| 0xC010 | 0xAA000C10 | Chip enable |
| 0xC014 | 0xAA000C14 | Command register 1 |
| 0xC020 | 0xAA000C20 | Command/address register |
| 0xC060 | 0xAA000C60 | Transfer kick |
| 0xC064 | 0xAA000C64 | Transfer mode |
| 0xC068 | 0xAA000C68 | Boot ECC input |
| 0xC0A0 | 0xAA000CA0 | Boot ECC output 0 |
| 0xC0A4 | 0xAA000CA4 | Boot ECC output 1 |
| 0xC0A8 | 0xAA000CA8 | Boot ECC output 2 |
| 0xC0AC | 0xAA000CAC | Boot ECC output 3 |
| 0xC0C0 | 0xAA000CC0 | STATUS2 |

### VRC4173 Framebuffer

| Address | Kseg1 Address | Size | Description |
|---|---|---|---|
| 0x0A200000 | 0xAA200000 | ~160 KB | Video memory, 320x240 @ 16bpp |

## Boot ROM Vectors

The 16 KB boot ROM is at PA `0x1FC00000`. In BEV (Boot Exception Vector) mode, exception vectors are mapped into ROM space:

| Offset | Physical Address | Kseg1 Address | Vector |
|---|---|---|---|
| 0x000 | 0x1FC00000 | 0xBFC00000 | Reset / NMI |
| 0x200 | 0x1FC00200 | 0xBFC00200 | TLB Refill (BEV) |
| 0x280 | 0x1FC00280 | 0xBFC00280 | XTLB Refill (BEV) |
| 0x300 | 0x1FC00300 | 0xBFC00300 | Cache Error (BEV) |
| 0x380 | 0x1FC00380 | 0xBFC00380 | General Exception (BEV) |

After NK.exe cold-start initialization, exception handlers are installed at the normal (non-BEV) vector locations in SDRAM:

| Physical Address | Kseg0 Address | Handler |
|---|---|---|
| 0x00000000 | 0x80000000 | TLB Refill |
| 0x00000100 | 0x80000100 | Cache Error |
| 0x00000180 | 0x80000180 | General Exception |

## SDRAM Key Data Structures

These fixed locations in SDRAM are used by the boot ROM and NK.exe for state management:

| Physical Address | Size | Description |
|---|---|---|
| 0x00001000 | 4 KB | Page table (zeroed by cold-start) |
| 0x000017E0 | -- | Initial stack pointer (kseg1: 0xA00017E0) |
| 0x000018C0 | 256 B | Section table (64 entries, 4 bytes each) |
| 0x00001AC8 | 4 B | Kernel data pointer (0x80000000) |
| 0x00001ACC | 4 B | Kernel code pointer (0x8008B84C) |
| 0x00002200 | 256 B | resume_ctx -- GPR and CP0 save area |
| 0x00002400 | 4 B | Version marker (expected: 0x03020100) |
| 0x000024FC | 4 B | NK.exe entry point (written by ROM) |
| 0x00002524 | 4 B | Hibernate signature (0x3210xxxx = valid) |
| 0x0000254C | 4 B | Hibernate flags (bits 0x03 gating) |
| 0x00003800 | -- | Boot stack pointer (kseg0/kseg1) |

### resume_ctx Layout (PA 0x2200)

The resume context is used by NK.exe to save and restore processor state across hibernate/resume cycles:

**GPR Section (offset 0x00 - 0x7C):**

| Offset | Register | Offset | Register |
|---|---|---|---|
| 0x00 | $at (1) | 0x04 | $v0 (2) |
| 0x08 | $v1 (3) | 0x0C | $a0 (4) |
| 0x10 | $a1 (5) | 0x14 | $a2 (6) |
| 0x18 | $a3 (7) | 0x1C | $t1 (9) |
| ... | ... | ... | ... |
| 0x6C | $sp (29) | 0x70 | $fp (30) |
| 0x74 | $ra (31) | 0x78 | HI |
| 0x7C | LO | | |

!!! note "$t0 Excluded"
    Register $t0 (reg 8) is skipped in the resume_ctx save -- it is used as a scratch register during the restore sequence itself.

**CP0 Section (offset 0x80 - 0xD0):**

| Offset | CP0 Register | Offset | CP0 Register |
|---|---|---|---|
| 0x80 | Index | 0x84 | Random |
| 0x88 | EntryLo0 | 0x8C | EntryLo1 |
| 0x90 | Context | 0x94 | PageMask |
| 0x98 | Wired | 0x9C | Count |
| 0xA0 | EntryHi | 0xA4 | Compare |
| 0xA8 | Status | 0xAC | Cause |
| 0xB0 | EPC | 0xB4 | Config |
| 0xB8 | LLAddr | 0xBC | WatchLo |
| 0xC0 | XContext | 0xC4 | ECC |
| 0xC8 | TagLo | 0xCC | TagHi |
| 0xD0 | ErrorEPC | | |
