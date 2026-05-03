# PC Connect Bridge

`--pcconnect-bridge` connects the emulated VRC4173 SIU UART to a host-side
character device. This models the BE-300 cradle correctly: the cradle is a
USB-to-UART bridge from the PC's point of view, while WinCE on the BE-300 talks
to a plain 115200 8N1 serial port.

## Why UART, Not USB Gadget Emulation

The BE-300 does not expose a USB device controller to WinCE. PC Connect reaches
the device through the dock UART. A host VM can put a USB-serial adapter in
front of the emulator's TCP/Unix/PTY endpoint, but the guest-visible hardware
remains the VRC4173 SIU.

## CLI

```text
--pcconnect-bridge <S>   S = tcp:HOST:PORT
                           | tcp-listen:PORT[@ADDR]
                           | unix:/PATH
                           | unix-listen:/PATH
                           | pty:auto
--pcconnect-tee <P>      Annotated bidirectional byte log
--pcconnect-baud <N>     Guest-to-host throttle, default 115200, 0 unlimited
```

`--pcconnect-bridge` is mutually exclusive with `--pcconnect-time-sync` and
`--stowaway-keyboard`; all three are peers on COM1.

## Example

```bash
./build-host/be300 --nand ce/restore_images/All_nand_300.bin \
  --pcconnect-bridge tcp:127.0.0.1:5555 \
  --pcconnect-tee /tmp/pcc.tee
```

Set `BE300_PCC_TRACE=1` to log bridge state changes on stderr.

## Tee Log

The tee file records both byte directions:

```text
<mono_ns> H>G <count>: <hex bytes>
<mono_ns> G>H <count>: <hex bytes>
<mono_ns> H>G:drop <count>: <hex bytes>
```

`H>G:drop` means host bytes arrived before the guest-side dock-detect edge and
were dropped to mimic an unpowered/unconnected cradle.

## VM Setup Pattern

One tested pattern is:

1. Configure a Windows 2000/XP VM serial device as TCP server on
   `127.0.0.1:5555`.
2. Use a USB-serial style device in the VM so Windows sees a COM port.
3. Start the emulator with `--pcconnect-bridge tcp:127.0.0.1:5555`.
4. Launch Casio PC Connect in the VM and point it at the new COM port.

For protocol work, compare the tee output with real PortMon captures using
`tools/pcconnect_diff_tee.py`.

## Current Limitation

The transport path moves bytes in order between the host fd and guest UART. The
remaining known blocker is guest-side protocol behavior: AtPcCnct/COShell paths
currently emit wake/ready bytes but not the full multi-byte framed responses
seen in real hardware captures. Treat future work here as WinCE-side protocol
reverse engineering, not as USB emulation.
