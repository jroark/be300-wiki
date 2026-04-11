# Casio Cassiopeia BE-300 Wiki

The **Casio Cassiopeia BE-300** is a Windows CE-based pocket PC released in 2002. Built around the NEC VR4131 MIPS processor at 166 MHz with 16 MB of SDRAM and 16 MB of NAND flash, it features a 320x240 color STN display and runs Windows CE 3.0 out of the box.

This wiki documents the hardware architecture, Windows CE firmware internals, the historical Linux4.BE porting effort (2002--2004), and a modern cycle-accurate emulator that boots the device from its ROM reset vector through the full cold-boot sequence.

<div class="grid cards" markdown>

-   **Hardware Reference**

    ---

    NEC VR4131 SoC, VRC4173 companion chip, memory map, NAND flash, framebuffer, GPIO, and board photos.

    [:octicons-arrow-right-24: Hardware](hardware/index.md)

-   **Windows CE Internals**

    ---

    Boot ROM, SPL bootloader, NK.exe cold boot, NAND layout, restore images, PPSH debug shell, and hibernate/resume.

    [:octicons-arrow-right-24: Windows CE](wince/index.md)

-   **Linux4.BE Archive**

    ---

    Historical documentation of the Linux port: bootloader, kernels, ramdisks, toolchain, and PicoGUI.

    [:octicons-arrow-right-24: Linux4.BE](linux4be/index.md)

-   **Emulator Documentation**

    ---

    Architecture, building, running, GXemul integration, and analysis tools for the cycle-accurate BE-300 emulator.

    [:octicons-arrow-right-24: Emulator](emulator/index.md)

</div>

## The Device

![BE-300 running PicoGUI under Linux](images/linux4be/be-picogui-test1-02.jpeg){ width="320" }

The BE-300 was one of the few MIPS-based Windows CE handhelds to receive a community Linux port. The Linux4.BE project (2002--2004) produced working 2.4 and 2.6 kernels with serial console, framebuffer, and PicoGUI support, though hardware driver coverage remained limited.

## The Emulator

![Emulated WinCE boot](images/screenshots/All_nand_300.png){ width="320" }

The emulator performs a true cold boot from the 16 KB masked ROM at `0xBFC00000`, loading the SPL from NAND, decompressing NK.exe into RAM, and handing off to the Windows CE kernel -- exactly as real hardware does after battery removal. It uses GXemul 0.7.0 as its MIPS CPU engine with native CP0, TLB, and dyntrans JIT support.

## Key Specifications

| Parameter | Value |
|---|---|
| CPU | NEC VR4131, MIPS32 LE, 166 MHz |
| RAM | 16 MB SDRAM (2x Samsung K4S641633D) |
| Storage | 16 MB NAND (Samsung K9F2808U0B) |
| Display | 320x240, 16-bit color STN (EPSON L50753) |
| ROM | 16 KB masked ROM at PA 0x1FC00000 |
| OS | Windows CE 3.0 (upgradeable to .NET 4.x) |
| Companion | NEC VRC4173 |
| Connectivity | RS-232C serial, headphone jack, CompactFlash slot |
