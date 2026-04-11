# Glossary

Hardware acronyms, MIPS terminology, and BE-300-specific terms used throughout this wiki.

---

**BCU** -- Bus Control Unit. The VR4131 bus arbiter that manages chip select lines, wait states, and memory region configuration.

**BEV** -- Bootstrap Exception Vector. A bit in the CP0 Status register. When set, exception vectors point to ROM addresses (base `0xBFC00000`) instead of RAM (base `0x80000000`). Set during early boot, cleared once NK.exe installs its own handlers.

**B000FF** -- A binary container format used by Windows CE on the BE-300. The SPL bootloader and NK.exe kernel image are both packaged in B000FF format. Structure: 7-byte signature (`B000FF\n`), followed by image start address, image length, then records of `[address + length + checksum + data]`.

**CMU** -- Clock Management Unit. Controls clock distribution and frequency dividers for VR4131 peripherals.

**COPYentry** -- A record in the Windows CE ROMHDR that describes a data section to be copied from the NK.exe image into RAM at boot. Contains source address, destination address, copy length, and total length (including BSS zero-fill).

**CP0** -- Coprocessor 0. The MIPS system control coprocessor responsible for TLB management, exception handling, memory configuration, and processor status. Key registers include Status, Cause, EPC, Config, EntryHi, EntryLo0/1, PageMask, and PRId.

**CyaCE** -- A CompactFlash-based bootloader that enables loading Linux kernels on the BE-300 from a CF card while running Windows CE.

**dyntrans** -- Dynamic binary translation. GXemul's JIT compilation technique that translates frequently executed MIPS instructions into host-native code for improved performance.

**GIU** -- General-purpose I/O Unit. The VR4131's GPIO controller for reading and writing digital pins, with configurable direction and interrupt support.

**ICU** -- Interrupt Control Unit. Manages interrupt routing in the VR4131, with status registers, mask registers, and write-one-to-clear acknowledgment. Cascades VRC4173 interrupts.

**kseg0** -- MIPS cached unmapped kernel segment. Virtual address range `0x80000000`-`0x9FFFFFFF`. Physical address = VA minus `0x80000000`. No TLB translation required. Used for most kernel code and data.

**kseg1** -- MIPS uncached unmapped kernel segment. Virtual address range `0xA0000000`-`0xBFFFFFFF`. Physical address = VA minus `0xA0000000`. No TLB translation, no caching. Used for hardware register access.

**kuseg** -- MIPS user segment. Virtual address range `0x00000000`-`0x7FFFFFFF`. Requires TLB translation to map virtual pages to physical frames. Used for user-mode code and data.

**MIPS16** -- A compact 16-bit instruction encoding for the MIPS architecture. Instructions are half the size of standard 32-bit MIPS instructions. The BE-300 boot ROM contains approximately 5.5KB of MIPS16 code (34 functions). Mode switching between MIPS32 and MIPS16 uses JALX instructions.

**NAND** -- Not-AND flash memory. The BE-300 uses a Samsung K9F2808U0B 16MB NAND flash chip for storing the operating system (SPL, NK.exe kernel, FAT16 filesystem).

**NK.exe** -- The Windows CE kernel image stored in NAND flash. "NK" stands for "NAND Kernel." Contains the kernel proper, OEM Adaptation Layer, device drivers, and 95 XIP modules. Loaded at VA `0x80060000`, approximately 6.2MB in size.

**OAL** -- OEM Adaptation Layer. The hardware abstraction interface in Windows CE that bridges the OS kernel and the specific hardware platform. On the BE-300, OAL code handles VR4131/VRC4173 initialization, interrupt routing, timer setup, and display management.

**OOB** -- Out-of-Band data. Spare bytes stored alongside each NAND flash page (16 bytes per 512-byte page on the K9F2808U0B). Contains: `0xAA 0x55` (marker), `0x0F` (status), `0xFF` (reserved), 4-byte logical block ID, and 8-byte ECC data.

**PIU** -- Peripheral Interface Unit. The VRC4173's touchscreen controller that handles analog-to-digital conversion of touch panel coordinates.

**PMU** -- Power Management Unit. Controls power states, standby mode, and wake-up sources on the VR4131.

**PPSH** -- Parallel Port Shell. A Casio debug interface accessible at PA `0x0C000120`. When detected by NK.exe during boot, it redirects console I/O to a debug shell (bypassing the normal GUI boot). The emulator supports PPSH via the `--ppsh` flag.

**PRId** -- Processor Identification register (CP0 register 15). Contains the processor type and revision. The VR4131 reports `0x00000C81`.

**pTOC** -- Pointer to Table of Contents. The address of the ROMHDR structure within NK.exe. Located at `0x80655C54` in the WinCE 3.0 image. Found at NK.exe file offset `0x44`.

**resume_ctx** -- A GPR and CP0 register save area at PA `0x2200`-`0x22FF`. Used by the hibernate/resume mechanism. On warm boot, saved state is restored from this area. On cold boot, the emulator seeds it with cold-start values since no prior save exists.

**ROMHDR** -- ROM Header. A structure in the NK.exe image that describes the kernel layout: physical address range, number of modules, RAM start address, and COPYentry records. Identified by the "ECEC" signature at NK.exe offset `0x40`.

**RTC** -- Real-Time Clock. Maintains time-of-day and provides periodic timer interrupts (RTCL1 at 100Hz) used for OS scheduling.

**SIU** -- Serial Interface Unit. The VR4131's UART controller. An 8250/16550-compatible serial port is provided by the VRC4173 at PA `0x0A008680`.

**SPL** -- Secondary Program Loader. Also called "Kloader." A small bootloader (~49KB) stored in B000FF format at NAND offset `0x4000`. Loaded by the boot ROM into VA `0x80F00000`. Its primary task is decompressing the NK.exe kernel image from NAND into RAM.

**TLB** -- Translation Lookaside Buffer. A 32-entry cache in the MIPS CPU that maps virtual page numbers to physical frame numbers. Supports variable page sizes from 4KB to 16MB via the PageMask register. TLB misses generate refill exceptions handled by software.

**VR4131** -- NEC MIPS System-on-Chip used in the Casio BE-300. Integrates a MIPS32 CPU core with on-chip peripherals (BCU, ICU, CMU, PMU, RTC, GIU, SIU, PCI controller).

**VRC4173** -- NEC companion chip for the VR4131. Provides additional I/O: second interrupt controller, USB host, audio, touchscreen (PIU), PS/2 interface, and GPIO. Connected via the VR4131's local bus at base address `0x0A000000`.

**XIP** -- Execute In Place. A technique where code runs directly from ROM or flash without being copied to RAM first. Despite the name, NK.exe's 95 "XIP modules" are not truly executed in place on the BE-300 -- the entire kernel is decompressed into SDRAM by the SPL before execution.
