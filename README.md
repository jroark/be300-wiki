# BE-300 Wiki

Static documentation wiki for the Casio Cassiopeia BE-300 handheld device, covering hardware, Windows CE firmware, the Linux4.BE porting project, and a modern emulator.

Live at **https://wiki.linux4be.com**

## Contents

The wiki is organized into five sections:

### Hardware
NEC VR4131 CPU, VRC4173 companion chip, memory map, register reference, interrupt cascade, NAND flash controller, serial port, framebuffer, touchscreen, CompactFlash, GPIO, and board photos from disassembly.

### Windows CE
Boot ROM reverse engineering (16KB masked ROM with MIPS16 code), SPL/Kloader (B000FF container format), ROM-to-SPL-to-NK.exe handoff protocol, NK.exe cold boot flow, NAND partition layout, restore images, PPSH parallel port debug interface, flash tools, and hibernate/resume context.

### Linux4.BE
Historical archive of the Linux4.BE project (2002-2004) that ported Linux to the BE-300. Includes project history, getting started guide, CyaCE bootloader, kernel building (2.4/2.6), ramdisk creation, cross-compilation toolchain, GUI frameworks (PicoGUI, MicroWindows, X11, Qt/Embedded), downloads, blog archive, and community resources.

### Emulator
Architecture guide, build instructions, current WinCE NAND boot workflow, source code layout, GXemul CPU engine integration (with MIPS16 support), web/Android frontends, PC Connect bridge notes, BEDiag diagnostic tool, and analysis scripts.

### Reference
Datasheet index, ground truth hardware register dumps from real hardware, NK.exe memory layout, and glossary.

### Archive
The original linux4.be website (circa 2002-2004) is preserved as browsable static HTML under `/static/archive/linux4be/`.

## Building

Requires Python 3.x and MkDocs with the Material theme.

```bash
pip install mkdocs-material
```

### Local development

```bash
mkdocs serve
```

Opens a local dev server at http://127.0.0.1:8000 with hot reload.

### Build static site

```bash
mkdocs build
```

Produces a self-contained static site in `site/`. The output can be served from any static file host (S3, Netlify, Cloudflare Pages, GitHub Pages, or `python3 -m http.server`).

### Deploy to production

```bash
mkdocs build
aws s3 sync site/ s3://BUCKET_NAME --delete
aws cloudfront create-invalidation --distribution-id DIST_ID --paths "/*"
```

## Project structure

```
be300-wiki/
  mkdocs.yml                    # MkDocs configuration and navigation
  docs/
    index.md                    # Home page
    hardware/                   # 13 pages: CPU, companion chip, memory map, etc.
    wince/                      # 10 pages: boot ROM, SPL, NK.exe, NAND, PPSH, etc.
    linux4be/                   # 10 pages: history, guides, blog, community
    emulator/                   # emulator pages: architecture, building, running, tools, frontends
    reference/                  # 5 pages: datasheets, ground truth, glossary
    images/
      hardware/                 # Board photos from disassembly
      screenshots/              # Emulator boot screenshots
      linux4be/                 # Historical photos from the Linux4.BE project
    static/
      archive/linux4be/         # Full linux4.be website archive (~128 MB)
```

## Content sources

Wiki content is synthesized from:

- **[be300-framebuffer](https://github.com/jroark/be300)** -- Emulator source, hardware documentation, reverse engineering notes, boot ROM analysis, register dumps from real hardware
- **linux4.be archive** -- Wayback Machine capture of the original Linux4.BE project website (2002-2004), including technical pages, blog posts, IRC logs, forum, and downloads
- **Real BE-300 hardware** -- Register dumps, TLB snapshots, memory diffs, and diagnostic captures from a physical device

## Related projects

- [be300-framebuffer](https://github.com/jroark/be300) -- BE-300 emulator (GXemul-based, boots Linux and WinCE)
- [GXemul](https://github.com/jroark/GXemul) -- Forked MIPS CPU engine with MIPS16 and VR4131 support

## License

Documentation content is original work or adapted from the Linux4.BE project archive. Hardware datasheets and board photos are attributed to their original authors. The linux4.be archive is preserved for historical and educational purposes.
