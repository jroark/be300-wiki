# ROM-SPL Handoff

The transition from the boot ROM through the SPL and into NK.exe is mediated by a set of **mailbox addresses** in low SDRAM and a **callback table** populated by the ROM's MIPS16 boot dispatcher.

## Mailbox Contract

Two physical addresses in SDRAM serve as the communication channel between boot stages:

| Address | Purpose | Expected Value |
|---------|---------|----------------|
| PA `0x2400` | Version marker | `0x03020100` (checked by NK.exe at VA `0x76CBC`) |
| PA `0x24FC` | Next-stage entry address | NK.exe entry point (written by SPL) |

### Write Sequence

1. The **SPL** decompresses NK.exe into RAM starting at PA `0x60000`
2. The SPL writes the NK.exe entry address to PA `0x24FC` (with `0x20000000` mask, converting to a kseg1 address)
3. The SPL writes version marker `0x03020101` to PA `0x2400`
4. Control returns to the **boot ROM**

### Read Sequence

1. The ROM checks PA `0x2400` for cold/warm state
2. After completing section copying and callback registration, the ROM loads PA `0x24FC`
3. The ROM executes `JR` to the address found at PA `0x24FC`, entering NK.exe

!!! note
    The version marker at PA `0x2400` has a subtle difference: the SPL writes `0x03020101`, but NK.exe checks for `0x03020100`. The low byte difference does not affect the check at `0x76CBC`, which masks or ignores it. If the version check fails, NK.exe clears PA `0x254C`, which prevents the hibernate path from activating.

## SPL Callback Table

The SPL registers a callback table at VA `0x80F01590` with hooks for:

- **Open/init** -- initialize the NAND reading subsystem
- **Read/copy** -- read data from NAND into RAM
- **Seek** -- position within the NAND image
- **Metadata hooks** -- image metadata queries

These callbacks allow the ROM's boot dispatcher to invoke SPL NAND-reading functions during the late boot phase.

## MIPS16 Section Copier (0x9FC00C85)

Before NK.exe starts, the ROM's MIPS16 section copier processes the **COPYentry** table embedded in the NK.exe image:

- NK.exe has an `"ECEC"` signature at file offset `0x40`
- The pTOC pointer at offset `0x44` points to the ROMHDR at VA `0x80655C54`
- The ROMHDR contains 1 COPYentry:

| Field | Value |
|-------|-------|
| Source | `0x800BBA70` |
| Destination | `0x80660000` |
| Copy size | 1,029 bytes |
| Total size | 52,852 bytes (includes BSS zero-fill) |

This copies the kernel's initialized `.data` section and zeros the `.bss` section before NK.exe begins execution.

## MIPS16 Boot Dispatcher (0x9FC00C21)

The boot dispatcher runs after the section copier and performs two critical tasks:

### OEMInit Callback Table

The dispatcher populates a callback table at **PA 0x51680** with function pointers into NK.exe OAL code. This table contains:

- 11 callback groups
- 55 non-zero words (function pointers)
- 32 entries total, each 20 bytes (some entries are empty)
- Function pointers target addresses in the `0x8007xxxx` and `0x800Axxxx` ranges

These callbacks are invoked later by NK.exe's `OEMInit` function (dispatcher at `0x7AB38`), which walks the table and calls each registered function pointer via `JALR`.

### NAND Driver Initialization

The boot dispatcher also initializes the NAND driver state, reading partition descriptors via `FUN_9fc015dc` (which reads partition entry 1, the SPL location) and setting up data structures for later NAND access.

## Complete Handoff Timeline

```
ROM: CP0 init, HW init, cold/warm detection
ROM: Clear PA 0x2400/0x24FC (cold boot)
ROM: Set SP = 0x80003800, serial init
ROM: Load SPL from NAND into 0x80F00000
  SPL: HW init (VR4131, VRC4173)
  SPL: Read NK.exe from NAND, decompress to 0x80060000
  SPL: Write entry to PA 0x24FC, version to PA 0x2400
  SPL: Return to ROM
ROM: BINFS section copier (MIPS16) -- copy .data, zero .bss
ROM: Boot dispatcher (MIPS16) -- register callbacks at PA 0x51680
ROM: SIU poke + BCU read loop
ROM: Load PA 0x24FC → JR to NK.exe entry
```
