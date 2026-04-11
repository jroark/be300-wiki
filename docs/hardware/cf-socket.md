# CompactFlash Socket

The BE-300 has a Type I CompactFlash slot, managed through the VRC4173 companion chip via PCMCIA-compatible registers.

## Status Register

| Address | Kseg1 Address | Function |
|---|---|---|
| 0x0A001000 | 0xAA001000 | CF status register |

Status register values:

| Value | Meaning |
|---|---|
| 0x04 | Card inserted |
| 0x0C | Card removed (or no card) |

## Interrupt Path

CompactFlash events (card insertion, removal, I/O ready) generate interrupts through the GIRQ0 sub-cascade:

1. CF state change or I/O ready
2. VRC4173 signals via GIRQ0 sub-cascade
3. GIU line 0 asserts
4. GIUINTLREG (0xAF000088) bit 0 set
5. Handler reads `0xAA000004` for sub-source identification

Both state-change (insert/remove) and level (I/O ready) interrupts are supported.

## Usage

### CyaCE Bootloader

The CyaCE bootloader reads Linux kernels and ramdisk images from a FAT-formatted CompactFlash card. This was the primary method for booting Linux on the BE-300 from a running Windows CE session.

### NANDWRITER

The NANDWRITER recovery tool (`NANDWRITER.bin`) reads firmware images from a FAT-formatted CF card:

- `KLOADER.bin` -- SPL bootloader
- `All_nand.bin` -- NK.exe kernel + FAT16 filesystem
- `nk.cpk` -- Alternate compressed NK format

NANDWRITER writes these images directly to NAND flash to restore a non-booting device.

## Linux Support

The Linux4.BE kernels did not include CompactFlash driver support. CF was only accessible under Windows CE.
