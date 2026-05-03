# Running

The native emulator binary is normally built into `build-host/be300`. From the
emulator repository root:

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin
```

Serial output is written to stdout. Emulator diagnostics, MMIO logs, and boot
messages are written to stderr. When SDL2 is available, a host window displays
the 240x320 guest framebuffer.

## WinCE Cold Boot

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin
```

This starts from the ROM reset vector and follows the real cold-boot chain:

1. Boot ROM initializes CPU and board state.
2. ROM reads NAND and loads SPL/Kloader.
3. SPL reads and decompresses NK.exe.
4. ROM/NK handoff starts the WinCE kernel.
5. WinCE reaches first-boot UI, date/time, touch calibration, and shell paths.

For interactive testing with a current guest date:

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin --rtc-host-time
```

Without `--rtc-host-time`, the emulator leaves the RTC in the cold-boot/default
state so WinCE can prompt for date/time like real hardware after battery
removal.

## Bounded Smoke Test

WinCE does not exit by itself. Use GNU `timeout` or `gtimeout` for automated
runs:

```bash
cd build-host
gtimeout 60s ./be300 --nand ../ce/restore_images/All_nand_300.bin \
  > cold_stdout.log 2> cold_stderr.log
```

Exit code `124` from `gtimeout` is expected for a long-running boot.

## Display And Input

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin --frame --scale 2
```

| Option | Description |
|--------|-------------|
| `--frame` | Show the BE-300 bezel around the LCD |
| `--scale <N>` | Render scale, `1.0` through `4.0` |
| `BE300_AUDIO=0` | Mute the host-rendered buzzer without changing guest hardware state |
| `BE300_TOUCH_MIN_DWELL_MS=<N>` | Minimum guest pen-down time for host clicks |

Touch input is routed through the VRC4173 PIU model. Host clicks are coalesced
into stable guest pen sessions so WinCE calibration sees repeated coordinate
samples.

## Accessories

### CompactFlash Storage

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin --cf cf.img
```

The image must be a multiple of 512 bytes. The emulator exposes an ATA CF card
with a minimal CIS that WinCE can classify as fixed-disk storage.

### NANDWRITER Restore Path

```bash
./build-host/be300 --restore --cf cf.img
```

`--restore` uses the primary PCMCIA socket for the recovery/restore path, so it
cannot be combined with `--ne2000`.

### PCMCIA NE2000

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin --ne2000
```

Use `--net-mac aa:bb:cc:dd:ee:ff` to override the default unicast MAC. When
one `--cf` image is combined with `--ne2000`, that CF image is attached to the
secondary socket; current WinCE visibility for the secondary socket is limited.

### Stowaway Keyboard

```bash
./build-host/be300 --nand nand_stowaway.bin --stowaway-keyboard
```

This enables the COM1 serial keyboard dock model. The NAND image must contain
the Stowaway driver registration, for example one built by
`tools/inject_stowaway.py`.

### PPSH Debug Shell

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin --ppsh
```

`--ppsh` makes the WinCE PPSH probe succeed and routes console I/O to the
debug shell instead of the normal GUI path.

### PC Connect

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin \
  --pcconnect-bridge tcp:127.0.0.1:5555 \
  --pcconnect-tee /tmp/pcc.tee
```

`--pcconnect-time-sync`, `--pcconnect-bridge`, and `--stowaway-keyboard` all
claim the same VRC4173 SIU UART, so only one can be enabled at a time.

## Command Line Options

| Option | Description |
|--------|-------------|
| `--nand <image>` | Boot WinCE from a raw NAND restore image |
| `--restore` | Enter CompactFlash recovery boot mode; requires `--cf` |
| `--cf <image>` | Attach a FAT/ATA CompactFlash image; may be specified twice |
| `--ne2000` | Attach a PCMCIA NE2000 Ethernet card |
| `--net-mac <mac>` | Override NE2000 MAC address |
| `--sdram <MB>` | SDRAM size in MiB, default 16 |
| `--rtc-host-time` | Initialize guest RTC from host local time |
| `--ppsh` | Enable PPSH debug-shell probe |
| `--pcconnect-time-sync` | Enable experimental PC Connect time-sync peer |
| `--pcconnect-bridge <spec>` | Pipe COM1 to TCP, Unix socket, or PTY chardev |
| `--pcconnect-tee <path>` | Write PC Connect bridge traffic to an annotated log |
| `--pcconnect-baud <N>` | Throttle bridge guest-to-host bytes; default 115200, `0` unlimited |
| `--stowaway-keyboard` | Enable the serial keyboard dock model on COM1 |
| `--frame` | Show the BE-300 frame asset |
| `--scale <N>` | Host render scale from `1.0` to `4.0` |
| `--speed <N>` | Target million guest instructions/sec; `0` unthrottled |
| `--trace` | Print each executed instruction to stderr |
| `--log-mmio` | Log MMIO register reads/writes |
| `--mmio-coverage` | Log first hit per MMIO device/offset/op and shutdown table |
| `--detect-stall` | Emit `[BE300_STALL]` for sustained tight PC loops |
| `--stall-window=N` | Instructions per stall sampling bucket |
| `--stall-threshold=K` | Unique-PC threshold for stall detection |
| `--stall-wall-secs=T` | Sustained wall-seconds before stall report |

Run `./build-host/be300 --help` for the exact option text of the built binary.
