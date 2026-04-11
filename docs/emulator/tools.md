# Analysis Tools

The `tools/` directory contains Python scripts and utilities for analyzing emulator output, NAND images, and NK.exe binaries. Docker cross-tools provide MIPS disassembly and symbol analysis.

## Python Scripts

### extract_b000ff.py

Extracts B000FF format records from NAND images. The B000FF container format (signature `B000FF\n`) is used by both the SPL bootloader and the NK.exe kernel image. Each record contains an address, length, checksum, and data payload.

### scan_nk_producers.py

Scans the decompressed NK.exe binary for store instructions (SW, SH, SB) that write to specific virtual addresses. Used to find which code paths populate key data structures like the resume context at PA `0x2200` or the OEMInit callback table at PA `0x51680`.

### disasm_nk_ctx.py

Disassembles specified code regions within NK.exe, producing annotated MIPS assembly output. Useful for targeted analysis of specific functions without needing a full disassembly of the 6.2MB binary.

### compare_screenshot.py

Compares emulator screenshots against baseline images to detect boot progress:

```bash
python3 tools/compare_screenshot.py build/screenshot_20260101_120000.bmp
```

Baseline images include `Starting.bmp` (the "Starting..." splash screen) and `Initializing.bmp` (the "Initializing..." screen with progress bar). Reports pixel similarity percentage and which baseline matches.

### wince_divergence_diff.py / wince_divergence_report.py

Generate boot divergence reports by comparing emulator execution traces against expected behavior. Useful for identifying where emulated boot flow departs from the known real-hardware sequence.

### summarize_wince_tlb.py

Summarizes TLB state captured during WinCE boot, showing mapped virtual-to-physical address translations and page sizes.

### cleanup_hw_dump.py

Cleans and formats raw hardware register dumps for use as reference data.

## Docker Cross-Tools

The Docker container (`mips-dev`) provides two MIPS cross-toolchains for binary analysis:

### Linux ELF Toolchain (mipsel-linux-gnu)

For analyzing Linux kernel ELF binaries:

```bash
# Disassemble a Linux kernel
mipsel-linux-gnu-objdump -d /work/kernels/vmlinux-pgui-demo | less

# List symbols
mipsel-linux-gnu-nm /work/kernels/vmlinux-pgui-demo | sort
```

### WinCE PE Toolchain (mipsel-pe)

For analyzing Windows CE Portable Executable files (Binutils 2.21.1, patched via 7shi/1374792):

```bash
# Disassemble a WinCE executable
mipsel-pe-objdump -d /work/ce/loader.exe

# List PE symbols
mipsel-pe-nm /work/ce/loader.exe
```

### Raw Binary Disassembly

For flat binaries like the decompressed NK.exe or extracted SPL:

```bash
# Disassemble NK.exe (raw binary, no headers)
mipsel-linux-gnu-objdump -D -b binary -m mips:3000 -EL /work/docs/nk_decompressed.bin

# Disassemble extracted SPL
mipsel-linux-gnu-objdump -D -b binary -m mips:3000 -EL spl_flat.bin
```

!!! tip "Finding hardware references in disassembly"
    To locate code that accesses hardware registers, search for `LUI` instructions that load upper address bytes:
    ```bash
    # Find VRC4173 (0x0A/0xAA), VR4131 (0x0F/0xAF), ROM (0xBF) references
    grep -E "lui.*0x(0a|aa|af|bf)" spl_disasm.txt
    ```
