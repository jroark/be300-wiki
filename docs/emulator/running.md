# Running

The `be300` emulator supports two primary boot modes: Linux kernel boot and WinCE NAND cold boot.

## Linux Kernel Boot

### Linux 2.4 (PicoGUI Demo)

```bash
./be300 --cmdline "console=tty0 console=ttyS0,9600 root=/dev/ram" \
  --kernel kernels/vmlinux-pgui-demo
```

This boots a Linux 2.4.18 kernel with a PicoGUI desktop environment to userspace. The kernel runs indefinitely until terminated.

### Linux 2.6

```bash
./be300 --cmdline "console=tty0 root=/dev/ram" \
  --kernel kernels/vmlinux-2.6 --sfb-5bit-green
```

!!! note "Color correction"
    The `--sfb-5bit-green` flag is required for the 2.6 kernel to display correct framebuffer colors. The 2.6 kernel uses a different pixel format (5-bit green channel) than the 2.4 kernels.

!!! warning "2.6 kernel ignores --cmdline"
    The Linux 2.6 kernel's `prom_init()` does not read bootloader arguments. The `--cmdline` flag has no effect. The kernel's serial console is hardcoded to the UART at `0xAA008680`.

## WinCE Cold Boot

```bash
./be300 --nand ce/restore_images/All_nand_300.bin
```

This starts a full cold boot from the ROM reset vector, exactly as real hardware does after battery removal:

1. Boot ROM initializes hardware and reads NAND
2. SPL (Secondary Program Loader) is loaded and decompresses NK.exe
3. ROM's MIPS16 boot dispatcher sets up callback tables
4. NK.exe entry point is called

Available NAND images:

| Image | Contents |
|-------|----------|
| `All_nand_300.bin` | WinCE 3.0 (SPL v0.52) |
| `All_nand_Net.bin` | WinCE 4.0 (.NET) |
| `org_CE_30.bin` | WinCE 3.0 (SPL v0.60) |
| `BE500.bin` | BE-500 variant (SPL v0.62) |
| `CE_Net.bin` | WinCE 4.0 (SPL v0.62) |

### PPSH Debug Shell

```bash
./be300 --nand ce/restore_images/All_nand_300.bin --ppsh
```

The `--ppsh` flag enables the Parallel Port Shell debug interface. When enabled, WinCE routes console I/O to the debug shell instead of booting to the GUI, eventually presenting a `Windows CE>` prompt.

## Command Line Options

| Option | Description |
|--------|-------------|
| `--kernel <path>` | Load a Linux ELF kernel |
| `--nand <path>` | Boot from a NAND restore image (WinCE cold boot) |
| `--cmdline <string>` | Kernel command line arguments (Linux only) |
| `--sfb-5bit-green` | Use 5-bit green channel for framebuffer (Linux 2.6) |
| `--ram <size>` | Set RAM size |
| `--sdram` | Enable SDRAM mode |
| `--speed <mhz>` | Set CPU speed |
| `--trace` | Enable instruction tracing |
| `--log-mmio` | Log memory-mapped I/O accesses to stderr |
| `--ppsh` | Enable PPSH debug shell (WinCE only) |

## Output

- **Serial output** goes to **stdout** (kernel printk messages, WinCE debug output)
- **Emulator diagnostics** go to **stderr** (MMIO logs, boot checkpoints, warnings)
- **SDL window** shows the framebuffer in real-time

!!! tip "Non-interactive runs"
    For automated testing, use `gtimeout` (GNU coreutils) to kill the emulator after a set duration, since kernels run indefinitely:
    ```bash
    gtimeout 20s ./be300 --kernel kernels/vmlinux-pgui-demo \
      --cmdline "console=tty0 console=ttyS0,9600 root=/dev/ram" \
      > stdout.log 2> stderr.log
    ```

## Screenshots

The emulator saves a BMP screenshot on clean exit (ui_quit or `be300_stop`). Note that when the process is killed by a signal (e.g., `gtimeout` sending SIGTERM), no screenshot is saved.
