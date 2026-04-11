# Serial Port

The BE-300 exposes an RS-232C serial port through the TCX3110 level shifter, driven by the VRC4173 companion chip SIU (Serial Interface Unit).

## Configuration

| Parameter | Value |
|---|---|
| Controller | VRC4173 SIU |
| Base Address | PA 0x0A008680 (kseg1: 0xAA008680) |
| Compatibility | 8250/16550 |
| Register Stride | 4 bytes |
| Clock | 18.432 MHz |
| Default Baud | 9600 |

## Register Map

The SIU implements a standard 16550 UART register set with 4-byte register spacing:

| Offset | Register | Description |
|---|---|---|
| 0x00 | RBR/THR | Receive buffer / Transmit holding |
| 0x04 | IER | Interrupt enable |
| 0x08 | IIR/FCR | Interrupt identification / FIFO control |
| 0x0C | LCR | Line control |
| 0x10 | MCR | Modem control |
| 0x14 | LSR | Line status |
| 0x18 | MSR | Modem status |
| 0x1C | SCR | Scratch register |

When LCR bit 7 (DLAB) is set, offsets 0x00 and 0x04 become the divisor latch (DLL/DLM) for baud rate configuration.

!!! warning "VR4131 SIU is Not the External Serial Port"
    The VR4131 has its own on-chip SIU at `0xAF000800`, but the BE-300 uses the VRC4173 companion chip SIU at `0xAA008680` for the external RS-232C port. Code targeting the wrong SIU will not produce serial output.

## Linux Console Usage

The Linux4.BE kernels use the serial port for console output:

```
console=ttyS0,9600
```

The 2.6 kernel registers the serial console directly via `be300_console_init()` in `prom_init()`, hardcoding the UART address at `0xAA008680`.

!!! note "Serial Console and Userspace"
    The 2.4 Linux kernels have a known issue where the serial console driver blocks userspace writes. When testing with the emulator, avoid using `console=ttyS0` if userspace startup is needed -- use `console=tty0` (framebuffer console) instead, or omit the serial console entirely.
