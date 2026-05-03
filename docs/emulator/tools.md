# Analysis Tools

The emulator repository includes tools for NAND images, WinCE binaries, boot
traces, PC Connect logs, and Web/Android asset builds. These tools live in
`../be300-framebuffer/tools/`.

## Common Tools

| Tool | Purpose |
|------|---------|
| `be300_nand.py` | Inspect and manipulate BE-300 NAND images |
| `extract_b000ff.py` | Extract B000FF-format SPL/NK records |
| `extract_nk_lzss.py` / `nk_lzss.py` | Work with NK.exe compression streams |
| `extract_xip_modules.py` | Extract XIP modules from NK.exe |
| `scan_nk_producers.py` | Find MIPS stores that write selected NK/SDRAM addresses |
| `disasm_nk_ctx.py` | Targeted NK.exe disassembly around selected regions |
| `compare_screenshot.py` | Compare emulator screenshots with boot-progress baselines |
| `pcconnect_diff_tee.py` | Align emulator PC Connect bridge tee logs with PortMon captures |
| `inject_stowaway.py` | Build a NAND variant with Stowaway driver registration |
| `build_cf_restore_image.py` | Create CompactFlash recovery images |
| `build_web.sh` | Build the WebAssembly frontend bundle |

## Docker Cross-Tools

The `mips-dev` Docker environment provides MIPS toolchains for analysis:

```bash
docker compose build mips-dev
docker compose run --rm mips-dev /bin/bash
```

| Toolchain | Target |
|-----------|--------|
| `mipsel-linux-gnu` | Linux ELF and raw little-endian MIPS binaries |
| `mipsel-pe` | Windows CE PE binaries |

Examples:

```bash
mipsel-linux-gnu-objdump -D -b binary -m mips:3000 -EL /work/docs/nk_decompressed.bin
mipsel-pe-objdump -d /work/ce/bediag/BEDiag.exe
```

## Diagnostics From The Emulator

| Runtime option | Output |
|----------------|--------|
| `--trace` | Instruction trace to stderr |
| `--log-mmio` | MMIO reads/writes to stderr |
| `--mmio-coverage` | First-hit device/offset/op log plus shutdown table |
| `--detect-stall` | `[BE300_STALL]` report for sustained tight PC loops |
| `--pcconnect-tee <path>` | Annotated PC Connect byte stream log |

Use bounded runs with `gtimeout` or GNU `timeout`; WinCE normally continues
running after it reaches UI.

## Ground Truth Inputs

The analysis tools are usually cross-checked against:

- real hardware register dumps in `docs/hardware/`
- `docs/CURRENT_STATUS.md` in the emulator repo
- PortMon captures for PC Connect
- BEDiag captures from real WinCE 3.0 hardware
