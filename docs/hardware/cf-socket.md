# CompactFlash And PCMCIA

The BE-300 exposes a Type I CompactFlash/PCMCIA path through the VRC4173
companion chip. The emulator models three related cases:

- no inserted media
- ATA CompactFlash image
- PCMCIA NE2000 network card

## Status And Companion State

The companion status register at PA `0x0A001000` is part of the CF/PCMCIA
state observed by WinCE drivers.

| Address | Kseg1 address | Function |
|---------|---------------|----------|
| `0x0A001000` | `0xAA001000` | CF/PCMCIA status |

Real-hardware captures show that adapter/socket presence and media presence are
not the same state. The emulator's current no-`--cf` behavior models the
socket/adapter path without inserted media so WinCE does not probe a floating
CIS window as an unknown card.

## ATA CompactFlash

Attach an image:

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin --cf cf.img
```

The image must be a multiple of 512 bytes. The emulator provides:

- ATA taskfile read/write behavior
- minimal fixed-disk CIS tuples
- `IDENTIFY`, read-sector, write-sector, verify, diagnostic, and feature paths
- companion/status words seeded from hardware captures where known
- image writeback when guest writes modify the CF image

## NANDWRITER Restore Path

The recovery path uses the primary PCMCIA socket:

```bash
./build-host/be300 --restore --cf cf.img
```

This path exposes the boot-time CF taskfile window used by the ROM/restore
flow. Because it owns the primary socket, `--restore` cannot be combined with
`--ne2000`.

## PCMCIA NE2000

Attach the NE2000 card model:

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin --ne2000
```

Optional MAC override:

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin \
  --ne2000 --net-mac 10:20:30:00:00:10
```

The NE2000 model exposes a PCMCIA network CIS with a conventional `0x300`
I/O window, maps guest requests to emulated NE2000 registers, and uses GXemul
network helpers on native builds. The web build can also forward raw Ethernet
frames through a WebSocket bridge.

## Socket Rules

- `--restore` requires the primary socket.
- `--ne2000` uses the primary socket.
- One `--cf` image can be combined with `--ne2000`; it is moved to the
  secondary socket.
- Current secondary-socket MMIO decoding is limited, so WinCE visibility of a
  secondary CF image is not equivalent to primary-slot storage.

## Interrupt Path

CF/PCMCIA state-change and level events route through the VRC4173/VR4131
cascade:

1. card state, ATA ready, or NE2000 event changes
2. VRC4173 companion status/latch bits update
3. GIRQ0 cascade asserts through the GIU path
4. VR4131 ICU dispatches the corresponding SYSINTR path
5. WinCE PCMCIA/storage/network drivers acknowledge through companion writes

The emulator keeps card-state events pending until the guest consumes the
state-change cause.

## Linux Support

The historical Linux4.BE kernels did not provide a complete BE-300 CF driver.
The maintained emulator CF/PCMCIA work is primarily for WinCE, NAND restore,
and accessory regression testing.
