# Flash Tools

Several utilities exist for managing the BE-300's NAND flash contents, including recovery flashing, on-device OS installation, and data backup/restore.

## NANDWRITER.bin

The primary flash recovery tool.

| Property | Value |
|----------|-------|
| Format | WinCE B000FF application |
| Version | v0.67 |
| Base address | `0x80E00000` |
| Input | CompactFlash card (FAT formatted) |
| Output | Direct NAND hardware writes |

### Operation

NANDWRITER reads from a FAT-formatted CompactFlash card:

- **`KLOADER.bin`** -- SPL bootloader
- **`All_nand.bin`** -- NK.exe kernel image + FAT16 filesystem
- **`Area.dat`** -- optional configuration file
- **`nk.cpk`** -- alternate compressed NK image format

It writes three volumes to NAND:

1. **Kloader** (SPL) -- partition entry 1
2. **Kernel** (NK.exe) -- partition entry 2
3. **Disk** (FAT16) -- partition entry 3

!!! note
    NANDWRITER performs a **direct block copy** with no data transformation. The bytes in the restore image file are written to NAND exactly as-is. This has been confirmed via Ghidra decompilation of the NANDWRITER binary.

### OOB Format

NANDWRITER writes the following OOB (out-of-band) data for each 512-byte page:

| Offset | Size | Contents |
|--------|------|----------|
| 0 | 1 | `0xAA` |
| 1 | 1 | `0x55` |
| 2 | 1 | `0x0F` |
| 3 | 1 | `0xFF` |
| 4 | 4 | Logical block ID (little-endian) |
| 8 | 8 | ECC data |

## KLOADER.bin

A standalone SPL bootloader in B000FF format.

| Property | Value |
|----------|-------|
| Format | B000FF container |
| Version | v0.62 |
| Size | 48 KB |

This is the same SPL used by NANDWRITER when flashing to the Kloader partition. It can also be used independently for recovery scenarios.

## DevOSInstall.exe

An on-device OS installer for the BE-300.

| Property | Value |
|----------|-------|
| Architecture | MIPS WinCE |
| Dependency | `NANDAccess.dll` (from NAND FAT16 at offset `0x6DC4FB`) |

This tool runs on the BE-300 itself and can update the OS from within a running WinCE session.

## Setup.exe

A PC-side upgrade coordinator.

| Property | Value |
|----------|-------|
| Architecture | x86 Win32 |
| Protocol | ActiveSync |

Setup.exe runs on a Windows PC connected to the BE-300 via ActiveSync. It pushes `UpdateData.bin` to the device to coordinate firmware updates.

## DevRestore.exe / DevBackup.exe

Data backup and restore utilities.

| Tool | Architecture | Purpose |
|------|-------------|---------|
| `DevRestore.exe` | MIPS WinCE | Restore backed-up data to device |
| `DevBackup.exe` | MIPS WinCE | Back up device data |

## BootInSafeModeWithPCC.exe

A MIPS WinCE utility that forces the device into safe mode boot, likely for recovery or diagnostic purposes.
