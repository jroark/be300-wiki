# NAND Flash

The BE-300 uses a Samsung K9F2808U0B 16 MB NAND flash chip for persistent storage, holding the SPL bootloader, Windows CE kernel (NK.exe), and a FAT16 filesystem.

## Specifications

| Parameter | Value |
|---|---|
| Part | Samsung K9F2808U0B |
| Capacity | 16 MB |
| Blocks | 1004 |
| Pages per block | 32 |
| Bytes per page | 512 (data) + 16 (spare/OOB) |
| Total raw size | 1004 x 32 x 512 = 16,449,536 bytes |
| Erase unit | Block (16 KB) |

## Controller Registers

The NAND controller is part of the VRC4173 companion chip, with registers at offsets from `0x0A000000`:

| Offset | Kseg1 Address | Width | Function |
|---|---|---|---|
| 0xC010 | 0xAA000C10 | 32 | Chip enable |
| 0xC014 | 0xAA000C14 | 32 | Command register 1 |
| 0xC020 | 0xAA000C20 | 32 | Command/address register |
| 0xC060 | 0xAA000C60 | 32 | Transfer kick |
| 0xC064 | 0xAA000C64 | 32 | Transfer mode (4=write, 5=read stream) |
| 0xC068 | 0xAA000C68 | 32 | Boot ECC input |
| 0xC0A0 | 0xAA000CA0 | 32 | Boot ECC output 0 |
| 0xC0A4 | 0xAA000CA4 | 32 | Boot ECC output 1 |
| 0xC0A8 | 0xAA000CA8 | 32 | Boot ECC output 2 |
| 0xC0AC | 0xAA000CAC | 32 | Boot ECC output 3 |
| 0xC0C0 | 0xAA000CC0 | 32 | STATUS2 (transfer status) |
| 0xB000 | 0xAA00B000 | 32 | Stream data buffer (read after kick) |

!!! note "Buffer Register Behavior"
    The stream data buffer registers at 0xA4A0-0xA4AC and STATUS2 at 0xA4C0 return data only when a stream transfer is active. Reads before any transfer return zero.

## Single Page Read Sequence

The ROM NAND read function (MIPS16 at `0x9FC01A4C`) performs the following sequence to read one 512-byte page plus 16 bytes of OOB:

1. **Chip enable**: Write to `0xC010`
2. **Command**: Write read command via `0xC014` and `0xC020`
3. **Address**: Write 3-byte page address via `0xC020` (column, page low, page high)
4. **Kick**: Write to `0xC060` to start the transfer
5. **Mode**: Write 5 to `0xC064` (read stream mode)
6. **Data read**: Read 520 bytes (512 data + 8 OOB) from `0xB000` in a loop
7. **ECC feed**: Write data to `0xC068` (Boot ECC input) during read
8. **Status check**: Read STATUS2 at `0xC0C0`
9. **ECC read**: Read corrected ECC from `0xC0A0`-`0xC0AC`

## OOB (Out-of-Band) Format

Each page has 16 bytes of spare (OOB) data with the following layout:

| Offset | Size | Content |
|---|---|---|
| 0-1 | 2 bytes | Magic: 0xAA 0x55 |
| 2 | 1 byte | Flags: 0x0F |
| 3 | 1 byte | Padding: 0xFF |
| 4-7 | 4 bytes | Logical block ID |
| 8-15 | 8 bytes | ECC data |

The magic bytes `0xAA 0x55` with flags `0x0F` indicate a valid data page. The logical block ID provides the block translation mapping.

## Block Translation Layer

The ROM implements a simple block translation layer in MIPS16 code at `0x9FC01710`:

1. **Linear search**: Scans all physical blocks sequentially
2. **OOB check**: For each block, reads OOB metadata (expects 0x55 0xAA + 0x0F + block ID)
3. **Majority vote**: Checks OOB across 5 pages per block to determine the logical block ID by majority
4. **ECC verification**: After finding the target block, runs software ECC correction

This is a wear-leveling scheme: logical blocks can reside at any physical block location. The translation is rebuilt by scanning at each boot.

## Boot ECC Engine

The hardware ECC engine is used during ROM boot to verify NAND data integrity:

- **Input**: Raw page data is fed to `0xC068` (BOOT_ECC_IN) during the page read
- **Output**: Computed ECC syndromes are read from `0xC0A0`-`0xC0AC` (BOOT_ECC_OUT)
- **Syndrome interpretation**: Zero syndromes mean no bit errors (correct data). Non-zero syndromes indicate correctable or uncorrectable errors.

!!! warning "ECC Engine Must Return Zero Syndromes"
    The emulator returns zero syndromes from the ECC output registers because the emulated NAND data is bit-perfect (no bit errors). Previously, echoing the input ECC back as output caused the ROM software ECC correction routine (`FUN_9fc01828`) to misinterpret the data as having errors and corrupt it.

## Software ECC Correction

If the hardware ECC reports non-zero syndromes, the ROM calls a software correction routine at `0x9FC01828`:

1. **Bit permutation**: `FUN_9fc01980` permutes 8 ECC bytes
2. **Syndrome unpacking**: Extracts 10-bit syndrome values
3. **Reed-Solomon correction**: `FUN_9fc01c30` / `FUN_9fc01ca0` perform error correction

## Partition Layout

See [NAND Layout](../wince/nand-layout.md) for the logical partition structure of the NAND image.
