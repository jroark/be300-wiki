# NK.exe Cold Boot Flow

This page documents the WinCE 3.0 NK.exe execution path after the ROM and
SPL/Kloader have loaded and decompressed the kernel image.

## Entry Point

NK.exe entry is at VA `0x80076B50` in the decompressed image. The early path:

1. initializes CP0 state
2. jumps through the uncached kseg1 alias of the same code
3. sets an early stack
4. enables required clocks
5. calls ROM/OAL hardware initialization helpers

The current emulator target is to reach this point through the real ROM and
SPL path, not by directly patching or staging NK.exe.

## Early Hardware Init

The early OAL path configures the VR4131 and VRC4173 hardware expected by the
kernel:

- BCU, CMU, PMU, RTC, ICU, and GPIO state
- VRC4173 companion windows
- NAND controller access
- interrupt routing and exception vectors
- display/touch/serial support used by later drivers

Several low SDRAM locations act as boot mailboxes and kernel data structures,
including the next-stage entry mailbox at PA `0x24FC`.

## Kernel Initialization

After OAL setup, NK.exe builds kernel memory structures, installs exception
handlers, initializes XIP module metadata, and starts scheduler/shell paths.

Important structures include:

| Address | Purpose |
|---------|---------|
| `0x80060000` | NK.exe load base |
| `0x80076B50` | NK.exe entry |
| `0x80655C54` | ROMHDR / pTOC pointer used by kernel init |
| `0x80660000` | RAM start after the NK image |

The ROMHDR identifies the XIP modules that make up the WinCE image, including
kernel components, drivers, shell, and applications.

## Display Mechanism

The "Initializing..." and "Starting..." screens are produced at runtime:

- OAL/display code clears and blits framebuffer regions.
- GWE/font rendering populates scratch buffers.
- The framebuffer aperture is mapped through the VRC4173 at PA `0x0A200000`.

These strings should be treated as runtime rendering output, not as static text
or bitmap resources in the NAND image.

## First-Boot UI

On a native cold boot, the emulator leaves the RTC in the default first-boot
state so WinCE can ask for date/time. For interactive testing, `--rtc-host-time`
initializes the guest RTC from host local time before boot.

Touch calibration relies on PIU hardware behavior:

- pen-change interrupt state
- page 0/page 1 coordinate valid bits
- data-lost behavior when both page buffers are still pending
- stable repeated samples while the host pointer remains down

## Superseded Investigation Notes

Older notes described a forced warm-resume path and a resume-context workaround
as if they were required for cold boot. That is no longer the current model.
For maintained documentation, treat those as historical investigation artifacts
unless a new trace contradicts the current source and status docs.

Likewise, old Welcome.exe, launcher dependency, VirtualCopy, and framebuffer
alias theories are not current boot blockers.
