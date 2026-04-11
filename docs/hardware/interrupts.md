# Interrupts

The BE-300 uses a two-level interrupt architecture. The VR4131 Interrupt Control Unit (ICU) is the top-level dispatcher, receiving interrupt sources from both on-chip peripherals and the VRC4173 companion chip. VRC4173 interrupts cascade through the VR4131 GIU (General-purpose I/O Unit).

## Interrupt Cascade Overview

```
CPU IP[7:0]
  │
  ├── IP7: Count/Compare timer (CP0 internal)
  ├── IP3: ICU SYSINT1 / SYSINT2
  │     │
  │     ├── SYSINT1REG (0xAF000080)
  │     │     ├── bit 0: Battery low
  │     │     ├── bit 1: Power switch
  │     │     ├── bit 3: Elapsed timer (ETIMER)
  │     │     ├── bit 8: GIU interrupt ──────────┐
  │     │     └── (other sources)                │
  │     │                                        │
  │     └── SYSINT2REG (0xAF00009C)              │
  │           └── (additional sources)           │
  │                                              │
  │   ┌──────────────────────────────────────────┘
  │   │
  │   ▼  GIUINTLREG (0xAF000088)
  │      ├── bit 0: GIRQ0 (CF / companion) ──────┐
  │      ├── bit 1: GIRQ1                        │
  │      ├── bit 7: GIRQ7                        │
  │      ├── bit 9: GIRQ9 (touchscreen)          │
  │      ├── bit A: GIRQA                        │
  │      └── bit B: GIRQB                        │
  │                                              │
  │   ┌──────────────────────────────────────────┘
  │   │
  │   ▼  GIRQ0 sub-cascade (0xAA000004)
  │      ├── CF state change
  │      ├── CF level interrupt
  │      └── (other companion sources)
  │
  └── IP2-IP6, IP0-IP1: (other/software)
```

## Top-Level: SYSINT1REG (0xAF000080)

The SYSINT1REG register reflects the active interrupt sources for the VR4131.

| Bit | Source | Description |
|---|---|---|
| 0 | BATTINTR | Battery low interrupt |
| 1 | POWERINTR | Power switch interrupt |
| 3 | ETIMERINTR | Elapsed timer interrupt |
| 8 | GIUINTR | GIU cascade (from companion chip and GPIO) |

The corresponding mask register is MSYSINT1REG at `0xAF000088`. Setting a bit in the mask register enables that interrupt source.

!!! warning "ETIMER Mask"
    The MSYSINT1 ETIMER bit (bit 3) must not be force-enabled by the emulator. WinCE manages its own timer interrupt mask -- forcing it on prevents the OS from controlling timer delivery.

## GIU Interrupt Cascade

When SYSINT1REG bit 8 (GIUINTR) is asserted, the interrupt handler reads GIUINTLREG (`0xAF000088`) to determine which GIU line triggered:

| Bit | GIU Line | Source |
|---|---|---|
| 0 | GIRQ0 | CompactFlash / VRC4173 sub-cascade |
| 1 | GIRQ1 | VRC4173 peripheral |
| 7 | GIRQ7 | VRC4173 peripheral |
| 9 | GIRQ9 | Touchscreen (pen-down) |
| 0xA | GIRQA | VRC4173 peripheral |
| 0xB | GIRQB | VRC4173 peripheral |

## GIRQ0 Sub-Cascade (0xAA000004)

GIRQ0 is itself a cascade from VRC4173 peripherals. The handler reads the status register at `0xAA000004` to dispatch further:

- CompactFlash state change (insert/remove)
- CompactFlash level interrupt (I/O ready)
- Other VRC4173 sources

## IRQ Acknowledge Registers

Both IRQ0 and timer interrupt handlers write 1 to the following VRC4173 registers to acknowledge/clear interrupt state:

| Address | Register |
|---|---|
| 0xAA001120 | VRC4173 interrupt acknowledge 1 |
| 0xAA00112C | VRC4173 interrupt acknowledge 2 |
| 0xAA001B20 | VRC4173 interrupt acknowledge 3 |

## WinCE SYSINTR Mapping

Windows CE maps hardware interrupts to logical SYSINTR values used by device drivers:

| SYSINTR | Value | Hardware Source |
|---|---|---|
| SYSINTR_KEYBOARD | 0x10 | Keyboard/button matrix |
| SYSINTR_TOUCH | 0x11 | Touchscreen pen-down (GIU Intr 9) |
| SYSINTR_TOUCH_CHANGED | 0x18 | Touchscreen position change |

### Touch Interrupt Path

1. Pen touches screen
2. VRC4173 PIU asserts pen-down signal
3. GIU line 9 triggers
4. GIUINTLREG bit 9 set
5. SYSINT1REG bit 8 (GIUINTR) asserts
6. ICU dispatches to GIU handler
7. Handler reads `0xAA000304` (pen-down register)
8. Returns SYSINTR_TOUCH (0x11) or SYSINTR_TOUCH_CHANGED (0x18)

### PCMCIA (CompactFlash) Interrupt Path

1. CF card inserted/removed or I/O ready
2. VRC4173 asserts via GIRQ0 sub-cascade
3. GIU line 0 triggers
4. GIUINTLREG bit 0 set
5. Handler reads `0xAA000004` for sub-source
6. Dispatches to CF state or level handler

## Timer Interrupt

The VR4131 has two timer sources relevant to WinCE:

1. **CP0 Count/Compare**: Standard MIPS timer, fires when Count reaches Compare. Used by WinCE scheduler.
2. **RTC Elapsed Timer**: VR4131 RTC elapsed time compare at `0xAF000100`. Fires ETIMERINTR (SYSINT1REG bit 3). Acknowledged by writing to RTCINTREG (`0xAF00011E`, write-1-to-clear).

## Interrupt Handler Installation

During NK.exe cold-start at VA 0x8007B398, exception handlers are copied to their runtime locations:

| Source VA | Destination PA | Handler |
|---|---|---|
| 0x8008C418 | 0x00000000 | TLB Refill |
| 0x8008B240 | 0x00000180 | General Exception (includes interrupt dispatch) |
| 0x800A8438 | 0x00000100 | Cache Error |

The general exception handler at PA 0x0180 checks CP0 Cause ExcCode to distinguish interrupts (ExcCode=0) from other exceptions, then dispatches to the appropriate interrupt handler based on the pending interrupt bits in Cause and the ICU status registers.
