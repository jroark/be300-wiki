# NAND Layout

The BE-300 uses a 16MB NAND flash chip organized as **1004 blocks x 32 pages x 512 bytes** per page, totaling **16,449,536 bytes**.

## Partition Table

The partition table resides in **block 0** of the NAND. Each entry is 16 bytes:

| Bytes | Contents |
|-------|----------|
| 0-7 | Padding (`0xFF` x 8) |
| 8-11 | Start sector (4 bytes, little-endian) |
| 12-15 | Size in sectors (4 bytes, little-endian) |

## Partitions

| Entry | Sectors | Offset | Size | Contents |
|-------|---------|--------|------|----------|
| 0 | 0 -- 31 | `0x00000` | 16 KB | Boot metadata |
| 1 | 32 -- 159 | `0x04000` | 64 KB | SPL / Kloader (B000FF format) |
| 2 | 160 -- 7,583 | `0x14000` | ~3.6 MB | NK.exe kernel (compressed, B000FF) |
| 3 | 7,584 -- 32,127 | `0x3B5000` | ~12 MB | FAT16 filesystem |

## Boot Metadata (Entry 0)

The first 16 KB contains the partition table itself along with boot flags and configuration data used by the ROM and SPL during the boot process.

## SPL Partition (Entry 1)

Contains the second-stage bootloader in B000FF container format. The `B000FF\n` signature appears at the start of the partition (NAND offset `0x4000`). See [SPL / Kloader](spl.md) for format details.

## NK.exe Partition (Entry 2)

Contains the compressed WinCE kernel image in B000FF format.

!!! note
    The NK partition at offset `0x14000` has a **single leading `0xFF` byte** before the `B000FF` signature. This is consistent across all five known restore images and is real data, not corruption.

## FAT16 Filesystem (Entry 3)

The FAT16 partition contains the user-accessible filesystem, including:

- System configuration files
- Installed applications
- User data
- The `NANDAccess.dll` library (at offset `0x6DC4FB` within the filesystem, used by `DevOSInstall.exe`)

## OOB (Out-of-Band) Data

Each 512-byte page has associated OOB metadata written by NANDWRITER:

| Offset | Size | Contents |
|--------|------|----------|
| 0 | 1 byte | `0xAA` |
| 1 | 1 byte | `0x55` |
| 2 | 1 byte | `0x0F` |
| 3 | 1 byte | `0xFF` |
| 4 | 4 bytes | Logical block ID |
| 8 | 8 bytes | ECC data |

The ROM's block translation layer (`FUN_9fc01710`) uses the OOB metadata to map logical blocks to physical blocks: it checks for the `0x55AA` + `0x0F` signature and the block ID, majority-voting across 5 pages per block to handle potential bit errors.

## Physical Geometry

| Parameter | Value |
|-----------|-------|
| Total size | 16,449,536 bytes (16 MB) |
| Blocks | 1,004 |
| Pages per block | 32 |
| Bytes per page | 512 (data) + 16 (OOB) |
| Total pages | 32,128 |
