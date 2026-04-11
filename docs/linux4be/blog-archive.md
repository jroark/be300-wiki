# Blog Archive

The linux4.be website included a news/blog section where Filip posted project updates. The archive below contains 10 of the 27 total entries that have been recovered. Entries are presented in chronological order.

---

## 2002

### February 19, 2002

Screen resolution bug fixed. The InspectorTux tool was updated with system information reporting capability, allowing users to dump hardware details from their BE-300 devices. This was one of the first steps in understanding the device hardware.

### February 20, 2002

Server access logs revealed that Casio Japan had visited the linux4.be website. The hardware manufacturer was aware of the Linux porting effort. No official contact or collaboration resulted, but the visits continued over time.

### March 11, 2002

A major milestone: custom flash ROM reading and modification capability was achieved. This gave the project the ability to examine the device firmware and understand the boot process, though the non-destructive CF card boot method meant ROM modification was not required for running Linux.

### March 15, 2002

A serial trace was captured during the factory restore process. The restore took approximately 45 minutes to complete, with serial output providing insight into the NAND flash writing sequence and WinCE image installation process.

### May 17, 2002

A link to the NEC VR4131 Preliminary User Manual was added to the project documentation. This was the primary reference for the BE-300's CPU and SoC peripheral programming.

### June 4, 2002

Memory configuration documented. A RAM/ROM/Flash FAQ was published covering the BE-300's memory layout, SDRAM configuration, and flash organization. This information was essential for kernel porting work.

### July 1, 2002

Serial driver progress reported. Kernel patches were posted online for community testing. The serial driver enabled console output during kernel boot, which was critical for debugging.

### July 5, 2002

The website was restructured into three sections targeting different experience levels:

- **Dummies** --- step-by-step instructions for end users
- **Developers** --- kernel building, toolchain setup, driver development
- **Hackers** --- hardware reverse engineering, register probing, low-level details

### December 9, 2002

Hardware mapping progress reported by jal0. A button keyboard driver was completed, enabling text input through the BE-300's physical buttons using the Rocket key as a modifier for character selection.

---

## 2003

### January 7, 2003

The "Dummies" howto was published, providing easy-to-follow instructions for non-technical users to boot Linux on the BE-300. This was a significant accessibility milestone --- for the first time, users who were not developers could try Linux on their device by simply copying files to a CF card and tapping an icon.

Community member frugalbrutus reported successfully testing the instructions.

---

!!! info "Incomplete Archive"
    This archive contains 10 of the 27 known blog entries. The remaining 17 entries have not yet been recovered from archive.org snapshots. The full linux4.be website is preserved in the [archive section](/static/archive/linux4be/) of this wiki and may contain additional entries.
