# NK.exe Memory Layout

NK.exe is the Windows CE 3.0 kernel image extracted from the NAND flash. It is loaded at physical address `0x60000` (virtual address `0x80060000` in kseg0) and occupies 6,253,256 bytes (approximately 6.2 MB).

## Address Map

```
0x80060000-0x80075FFF  Bootstrap and OAL data
0x80076B50-0x80079xxx  OAL initialization code
0x8007Axxx-0x8007Bxxx  OAL hardware drivers and cold-start entry
0x80080000-0x800FFFFF  Kernel proper (~640KB)
0x800A0000-0x800Bxxxx  OAL callbacks and device drivers
0x80060000-0x80656AC8  Total NK.exe image
0x80655C54             ROMHDR (pTOC)
0x80660000-0x81000000  RAM (ulRAMStart to ulRAMEnd)
```

## Key Code Addresses

| Address | Function |
|---------|----------|
| `0x80076B50` | NK.exe entry point (CP0 init, jump to kseg1) |
| `0x80078BC0` | OAL vtable initialization |
| `0x80078E10` | OAL display blit dispatcher |
| `0x80079460` | Pre-init continuation (calls init functions) |
| `0x800795D8` | Post-init OAL restore (always reached on cold and warm boot) |
| `0x8007B398` | Cold-start kernel entry (true cold init, builds everything from scratch) |
| `0x800947C8` | Kernel main init (`kernel_init(pTOC)`) -- loads 95 XIP modules |
| `0x8008B240` | General exception handler (copied to PA `0x0180`) |
| `0x8008BC18` | Default section handler |
| `0x8008C418` | TLB refill handler (copied to PA `0x0000`) |
| `0x800A8438` | Cache error handler (copied to PA `0x0100`) |

## ROMHDR (pTOC)

The ROM Header structure at VA `0x80655C54` describes the NK.exe image layout:

| Field | Value | Description |
|-------|-------|-------------|
| `physfirst` | `0x80060000` | Start of NK.exe in virtual memory |
| `physlast` | `0x80656AC8` | End of NK.exe image |
| `nummods` | 95 | Number of XIP modules in the image |
| `ulRAMStart` | `0x80660000` | Start of available RAM |

The "ECEC" signature at NK.exe offset `0x40` and the pTOC pointer at offset `0x44` identify the image as a Windows CE kernel.

## COPYentry

A single COPYentry record describes the kernel's initialized data section:

| Field | Value |
|-------|-------|
| Source | `0x800BBA70` |
| Destination | `0x80660000` |
| Copy length | 1,029 bytes |
| Total length | 52,852 bytes (includes BSS zero-fill) |

The ROM's MIPS16 section copier at `0x9FC00C85` processes this COPYentry before NK.exe starts, copying initialized data and zeroing BSS.

## Kernel Entry Table

Located at VA `0x80074D90`:

| Offset | Value | Purpose |
|--------|-------|---------|
| [0] | `0x8008CEA4` | Entry point 0 |
| [1] | `0x8009101C` | Entry point 1 |
| [2] | `0x80090F34` | Entry point 2 |
| [3] | `0x80090F40` | Entry point 3 |
| [4] | `0x80090F8C` | Entry point 4 |
| [5] | `0x80090FC8` | Entry point 5 |
| [6] | `0x80090FF4` | Entry point 6 |
| [7] | `0x00000000` | (unused) |
| +0x20 | `0x80655C54` | pTOC pointer |
| +0x2C | `"OEM\0"` | ASCII identifier |

## OEMInit Callback Table

Located at PA `0x51680` (VA `0x80051680`), this table contains 11 callback groups with 55 non-zero function pointer words. Each entry is 20 bytes (5 words). The function pointers reference NK.exe OAL code in the `0x8007xxxx` and `0x800Axxxx` ranges.

This table is populated by the ROM's MIPS16 boot dispatcher at `0x9FC00C21` before NK.exe begins execution.

## Key SDRAM Data Structures

| Address | Size | Contents |
|---------|------|----------|
| PA `0x1000` | 4 KB | Page table (zeroed by cold-start init) |
| PA `0x18C0` | 256 B | Section table (64 entries with handler pointers) |
| PA `0x1AC8` | 8 B | Kernel data pointers |
| PA `0x2200`-`0x22FF` | 256 B | resume_ctx -- GPR and CP0 save area for hibernate/resume |
| PA `0x2400` | 4 B | Version marker (`0x03020100` expected) |
| PA `0x24FC` | 4 B | NK.exe entry point (written by ROM mailbox function) |
| PA `0x2524` | 4 B | Hibernate signature (upper 16 bits = `0x3210` if valid) |
| PA `0x254C` | 4 B | Hibernate flags (bits `0x03` must be non-zero for hibernate path) |

## Exception Handlers

The cold-start init at `0x8007B398` installs real exception handlers by copying code from NK.exe to the fixed MIPS exception vector addresses:

| Vector Address | Handler Source | Purpose |
|---------------|---------------|---------|
| PA `0x0000` | `0x8008C418` | TLB refill |
| PA `0x0100` | `0x800A8438` | Cache error |
| PA `0x0180` | `0x8008B240` | General exception |
