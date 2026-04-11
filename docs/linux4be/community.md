# Community

## Communication Channels

The Linux4.BE project used several community platforms:

### IRC

The project IRC channel was **#linux4BE** on `irc.freenode.net`. This was the primary real-time communication channel for developers.

### Mailing List

The **linux4be-devel** mailing list hosted developer discussion, patch review, and announcements. The Pipermail archives of the mailing list have been preserved.

### Forum

A phpBB forum provided a space for user questions, bug reports, and general discussion about running Linux on the BE-300.

## Source Code Repository

The project used CVS for version control, hosted at `cvs.linux4.be`:

```
cvs -d:pserver:anonymous@cvs.linux4.be:/cvsroot co linux/kernel
```

The repository contained:

- Linux kernel source with BE-300 platform patches
- CyaCE bootloader source
- Ramdisk build scripts and configurations
- Documentation and HOWTOs

## Website

The original project website was hosted at **linux4.be**, administered by Filip from Belgium. The site served as the central hub for downloads, documentation, blog updates, and community coordination.

The website has been preserved from archive.org and is browsable in the [archive section](/static/archive/linux4be/) of this wiki.

## Contact

The project lead could be reached at **Filip@Linux4.BE**.

## Contributors

The project brought together an international group of developers:

| Contributor | Location | Primary Contribution |
|-------------|----------|---------------------|
| Filip | Belgium | Project lead, website, kernel builds |
| jroark (Antagonizt) | USA | Serial driver, ramdisks, PicoGUI |
| Andrey Shuvikov | --- | Hardware reverse engineering |
| jal0 | --- | Cross-toolchain, schematics, monitor program |
| Mouse | --- | Linux 2.6 kernel port |
| themaw (Marc) | --- | Kernel collaboration |
| Brian | --- | Contributor |

## Legacy

The Linux4.BE project was active from 2002 to 2004. While the original servers and communication channels are no longer online, the project produced the most thorough public documentation of the BE-300 hardware. The reverse engineering work --- particularly Andrey Shuvikov's interrupt, video, touchscreen, and serial documentation --- remains the foundation for the BE-300 emulator project.
