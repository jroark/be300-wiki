# Boot ROM

The BE-300 contains a **16KB masked ROM** at physical address **0x1FC00000**, accessible via kseg1 at **0xBFC00000** (uncached) or kseg0 at **0x9FC00000** (cached). The ROM has been dumped from real hardware (CRC32 = `0xFA3B5582`).

## Reset Vector

The MIPS reset vector is at VA `0xBFC00000`. The ROM begins with:

```
NOP
LUI/ORI  → 0xBFC002F0
JR       → 0xBFC002F0
```

This jumps to the main boot code at offset `0x02F0` within the ROM.

## ROM Layout

| Offset Range | Size | Contents |
|-------------|------|----------|
| `0x0000-0x00FF` | 256 B | Reset vector, exception stubs (MIPS32) |
| `0x0100-0x0C1F` | ~2.8 KB | Initialization and setup code (MIPS32) |
| `0x0C20-0x219B` | ~5.5 KB | MIPS16 function library (34 functions) |
| `0x219C-0x224F` | ~180 B | Function metadata + address table (34 entries at `0x21C0`) |
| `0x2250-0x3FFF` | ~7.6 KB | Unused padding (`0xFF` fill) |

## Mixed MIPS32/MIPS16 Code

The ROM is unusual in that it contains both **MIPS32** and **MIPS16** instruction encodings. The MIPS16 region at offsets `0x0C20-0x219B` contains 34 functions that handle the critical late-boot tasks: NAND reading, section copying, and boot dispatching.

Cross-mode calls use the **JALX** instruction:

- MIPS16 functions call MIPS32 helpers via JALX (jump-and-link-exchange)
- The BEV general exception handler at `+0x380` does `JALR` to `0x9FC00C85` (bit 0 = MIPS16 mode switch)

!!! note
    NK.exe is 100% MIPS32 -- no MIPS16 code exists anywhere in the 6.2MB kernel image. MIPS16 is only used within the boot ROM.

## MIPS32 Helper Functions

These MIPS32 functions are called from MIPS16 code via JALX:

| Address | Function |
|---------|----------|
| `0x9FC00464` | Mailbox writer -- writes NK.exe entry to PA `0x24FC`, version `0x03020101` to PA `0x2400` |
| `0x9FC00834` | memcpy-like |
| `0x9FC00888` | memset-like |
| `0x9FC00980` | Context save |
| `0x9FC009BC` | Context helper |
| `0x9FC00BC0` | Helper |
| `0x9FC00C04` | Trampoline -- pops s0, s1, a0 from stack, JR a0 |

## ROM NAND Functions (MIPS16)

The ROM contains a complete NAND reading stack implemented in MIPS16:

| Address | Function | Description |
|---------|----------|-------------|
| `0x9FC015F4` | Multi-page reader | Converts page addresses to logical blocks, caches last block |
| `0x9FC01710` | Block translation layer | Linear search of physical blocks, checks OOB metadata (`0x55AA` + `0x0F` + block ID), majority votes across 5 pages per block, software ECC post-search |
| `0x9FC019FC` | Block reader | Reads 32 pages (one block) via single page reader, sets success flag |
| `0x9FC01A4C` | Single page read | Chip enable (`0xC010`), command via `0xC014/0xC020`, 3-byte address, kick (`0xC060`), mode=5 (`0xC064`), reads 520+8 bytes from `0xB000`, HW ECC via `0xC068`, reads STATUS2 (`0xC0C0`), corrected ECC from `0xC0A0-0xC0AC` |
| `0x9FC01828` | Software ECC correction | Bit-permutes 8 ECC bytes, unpacks to 10-bit syndromes, Reed-Solomon correction |

## BEV Exception Vectors

The ROM occupies the BEV (Bootstrap Exception Vector) address space, but the vectors are not real exception handlers:

| Vector | Offset | Contents |
|--------|--------|----------|
| TLB refill | `+0x200` | All `0xFF` in original ROM (no handler) |
| General exception | `+0x380` | Boot code continuation (section copier + dispatcher) -- overlaps the BEV vector by coincidence |

!!! warning
    The code at `+0x380` is part of the boot flow, not a real exception handler. It just happens to be located at the BEV general exception vector address. The emulator patches the ROM at load time with proper BEV handlers: TLB refill at `+0x200`, general exception dispatcher at `+0x280`, and an EXL check at `+0x384` to distinguish exceptions from the boot flow.

## Boot Sequence

The ROM at `0xBFC002F0` executes these steps:

1. **CP0 init** -- initialize coprocessor 0 registers
2. **HW init** -- hardware initialization (`JALR 0x9FC006F0`)
3. **Cold/warm detection** -- check functions to determine boot type
4. **Cold boot setup** -- clear PA `0x2400/0x24FC`, additional init (`JAL 0xFC00734`)
5. **Stack and serial** -- set SP = `0x80003800`, serial init (`JAL 0xFC00498`)
6. **BINFS section copier** -- MIPS16 at `0x9FC00C85`, processes COPYentry
7. **Boot dispatcher** -- MIPS16 at `0x9FC00C21`, registers callbacks, NAND driver init
8. **SIU poke + BCU read** -- serial and bus checks (`JAL 0xFC004E8/0xFC00488`)
9. **Jump to NK.exe** -- load PA `0x24FC`, JR to NK.exe entry point

Steps 6-7 execute MIPS16 code natively. The ROM loads the SPL from NAND in the early stages; the SPL decompresses NK.exe; then the ROM continues with steps 5-9 to finalize the handoff.
