# Getting Started

!!! warning "Experimental Software"
    Linux on the BE-300 is experimental. Use at your own risk. However, this method is **non-destructive** --- Linux runs entirely from the CompactFlash card. Pressing the reset button returns the device to Windows CE with no permanent changes.

## Requirements

- Casio Cassiopeia BE-300 handheld
- CompactFlash card (any size supported by the BE-300)
- Linux4BE distribution archive (tarball or zip)

## Installation

1. Download the Linux4BE archive.
2. Extract the archive contents.
3. Copy the entire `Linux4BE` directory to the root of a FAT-formatted CompactFlash card.
4. Insert the CF card into the BE-300.

## Booting Linux

1. Using the WinCE file explorer, navigate to the `Linux4BE` directory on the storage card.
2. Double-tap `Autorun.exe` to launch the CyaCE bootloader.
3. The CyaCE menu presents several boot options:

| Menu Entry | Description |
|------------|-------------|
| **linux4be** | Boot to shell (command line) |
| **picogui** | Boot to PicoGUI graphical desktop |
| **linux4be-serial** | Boot with serial console output |

4. Select an entry and press OK to boot.

## Input Method

The BE-300 has limited physical buttons, so text input uses a character selection system built around the **Rocket** button.

### Hardware Button Mappings

| Button | Function |
|--------|----------|
| OK | Enter |
| ESC | Tab |
| PWR | Ctrl+Alt+Delete |
| Up / Down / Left / Right | Arrow keys |
| Rocket + OK | Space |
| Rocket + ESC | Backspace |
| Rocket + PWR | Ctrl+C |
| Rocket + Up | Next character |
| Rocket + Down | Previous character |
| Rocket + Right | Confirm selected character |
| Rocket + Left | Overwrite previous character |

To type text, hold the Rocket button and use Up/Down to cycle through characters, then press Right to confirm each character.

## Auto-Boot Configuration

To make the BE-300 automatically boot Linux when a CF card is inserted, rename the `Linux4BE` folder to `CE\R4100` on the CF card. The device auto-runs executables from this path on card insertion.

## CyaCE Configuration

The CyaCE bootloader reads a configuration file from the CF card. The format supports the following directives:

```
# Comment lines start with hash
timeout=300
image=vmlinux-pgui-demo
label=picogui
append=console=tty0 root=/dev/ram
```

| Directive | Description |
|-----------|-------------|
| `#` | Comment line |
| `timeout=N` | Boot timeout in deciseconds (300 = 30 seconds) |
| `image=filename` | Kernel image filename |
| `label=name` | Menu entry label |
| `append=params` | Kernel command line parameters |

Multiple `image`/`label`/`append` groups can be defined for a multi-entry boot menu.
