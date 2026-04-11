# Restore Images

Five NAND restore images have been preserved from the Casio BE-300 family. These are raw sector dumps in logical block order, each 16,449,536 bytes (1004 blocks x 32 pages x 512 bytes).

## Available Images

| Filename | OS Version | SPL Version | Notes |
|----------|-----------|-------------|-------|
| `All_nand_300.bin` | WinCE 3.0 | v0.52 | Primary development target |
| `org_CE_30.bin` | WinCE 3.0 | v0.60 | Alternate CE 3.0 image |
| `BE500.bin` | WinCE 3.0 | v0.62 | BE-500 model variant |
| `All_nand_Net.bin` | WinCE 4.0 (.NET) | v0.62 | CE .NET image |
| `CE_Net.bin` | WinCE 4.0 (.NET) | v0.62 | Alternate CE .NET image |

## Image Format

All images are **raw sector data** written by NANDWRITER with **identity block mapping** (no transformation between the image file and NAND contents). The format matches the [NAND layout](nand-layout.md) exactly:

- Offset `0x00000`: Boot metadata (16 KB)
- Offset `0x04000`: SPL / Kloader (64 KB, B000FF format)
- Offset `0x14000`: NK.exe kernel (compressed, ~3.6 MB)
- Offset `0x3B5000`: FAT16 filesystem (~12 MB)

## NK.exe Base Addresses

The decompressed NK.exe kernel loads at different base addresses depending on the OS version:

| OS Version | Base VA | Base PA |
|-----------|---------|---------|
| WinCE 3.0 | `0x80060000` | `0x00060000` |
| WinCE 4.0 (.NET) | `0x80029000` | `0x00029000` |

To convert a virtual address to a file offset within the decompressed NK.exe binary:

```
file_offset = VA - base_VA
```

## Primary Development Target

**All_nand_300.bin** (WinCE 3.0, SPL v0.52) is the primary target for emulator development and the most thoroughly reverse-engineered image. Key characteristics of its NK.exe:

| Property | Value |
|----------|-------|
| Decompressed size | 6,253,256 bytes (`0x5F6AC8`) |
| Image range | `0x80060000` -- `0x80656AC8` |
| ROMHDR (pTOC) | `0x80655C54` |
| XIP modules | 95 |
| ulRAMStart | `0x80660000` |
| SHA-256 | `df55a2f89c3c9635d0cf4f8bf73fa32ced70e7137fb00fcf7872b70a77f0b15f` |

!!! tip
    The emulator dumps the decompressed NK.exe to `nk_decompressed.bin` in the working directory during WinCE NAND boots. This file can be analyzed with standard MIPS disassembly tools.

## Creating Restore Images

Restore images were originally created using the NANDWRITER tool on real hardware. NANDWRITER reads from a FAT-formatted CompactFlash card containing `KLOADER.bin` (SPL) and `All_nand.bin` (NK + FAT16), then writes the three volumes (Kloader, Kernel, Disk) directly to NAND hardware registers. The write is a direct block copy with no data transformation.
