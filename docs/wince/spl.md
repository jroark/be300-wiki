# SPL / Kloader

The **SPL** (Second Program Loader), also called **Kloader** (Kernel Loader), is the second-stage bootloader stored in the NAND flash. It is responsible for reading and decompressing the NK.exe kernel image into RAM.

## B000FF Container Format

The SPL is stored at NAND offset `0x4000` in the **B000FF** container format:

### Header

| Offset | Size | Contents |
|--------|------|----------|
| 0 | 7 bytes | Signature: `B000FF\n` (ASCII, including newline) |
| 7 | 4 bytes | Image start address (little-endian) |
| 11 | 4 bytes | Image length (little-endian) |

### Records

Following the header, the image consists of multiple records:

| Field | Size | Description |
|-------|------|-------------|
| Address | 4 bytes | Destination VA for this record |
| Length | 4 bytes | Data length in bytes |
| Checksum | 4 bytes | Checksum of data |
| Data | Length bytes | Raw data to copy to destination |

The SPL image contains 8 records. The main code block is loaded at VA `0x80F01000` (0xB3BC bytes).

## Entry Point

The SPL loads at VA **0x80F00000** (PA 0x00F00000, kseg0, within the 16MB SDRAM). The entry stub is:

```
0x80F00000:  NOP
             LUI/ORI → 0x80F02404
             JR  0x80F02404
```

## Code Flow

Starting from the entry at `0x80F02404`:

1. **CP0 Config write** -- configures the cache
2. **Kseg1 switch** -- switches to uncached address space (`0xA0F0xxxx`) for safe hardware access
3. **Hardware initialization**:
    - VR4131 SoC registers at `0xAF00xxxx`
    - VRC4173 companion chip at `0xAA00xxxx` / `0xAA01xxxx`
    - Framebuffer at `0xAA20xxxx`

!!! note
    The SPL only writes CP0 Config (`$16`) and TagLo (`$28`). It does **not** set the Status register and relies on the boot-time value left by the ROM. It also does not set BEV=0 or install its own exception handlers.

## NK.exe Loading

The SPL loader core at `0x80F03B64` performs:

1. Reads the NK.exe **B000FF** container from NAND (starting at offset `0x14000`)
2. Decompresses the kernel image using Casio proprietary compression
3. Writes the NK.exe **entry address** to the mailbox at PA `0x24FC` (with `0x20000000` mask applied)
4. Writes **version marker** `0x03020101` to PA `0x2400`

After writing the mailbox, the SPL returns control to the boot ROM, which continues with section copying and the final jump to NK.exe.

## SPL Versions

Different NAND images contain different SPL versions:

| Image | SPL Version |
|-------|-------------|
| All_nand_300.bin | v0.52 |
| org_CE_30.bin | v0.60 |
| KLOADER.bin (standalone) | v0.62 |
| BE500.bin | v0.62 |
| CE_Net.bin | v0.62 |

The version string appears in the SPL binary as "Kernel loader core - Ver X.XX".
