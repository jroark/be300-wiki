# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

A static documentation wiki for the Casio BE-300 handheld, built with MkDocs Material. Live at https://wiki.linux4be.com. Hosted on S3 + CloudFront.

## Commands

```bash
# Local dev server with hot reload
mkdocs serve

# Build static site to site/
mkdocs build

# Deploy (build first, then sync + invalidate)
mkdocs build
aws s3 sync site/ s3://BUCKET --delete
aws cloudfront create-invalidation --distribution-id DIST_ID --paths "/*"
```

Prerequisite: `pip install mkdocs-material`

## Architecture

All content is Markdown under `docs/`. Navigation is defined explicitly in `mkdocs.yml` under the `nav:` key -- adding a new page requires both creating the `.md` file and adding it to `nav:`.

**Sections:**
- `docs/hardware/` -- BE-300 hardware (VR4131 CPU, VRC4173, memory map, peripherals)
- `docs/wince/` -- Windows CE firmware internals (boot ROM, SPL, NK.exe, NAND)
- `docs/linux4be/` -- Historical Linux4.BE project (2002-2004)
- `docs/emulator/` -- be300-framebuffer emulator docs
- `docs/reference/` -- Datasheets, register dumps, glossary

**Static assets:**
- `docs/images/` -- Wiki images (hardware photos, screenshots, historical)
- `docs/static/archive/linux4be/` -- Full linux4.be website archive (~128 MB, served as-is, do not modify)

**Each section has an `index.md`** that serves as the section landing page (enabled by `navigation.indexes` feature in mkdocs.yml).

## Content Conventions

- Use MkDocs Material admonitions (`!!! note`, `!!! warning`, `!!! tip`) for callouts
- Use fenced code blocks with language hints (```bash, ```mips, etc.)
- Reference images with relative paths: `../images/hardware/Board_top2.jpg`
- Hardware addresses use `0x` prefix consistently
- No emojis in content
- The linux4be archive under `docs/static/archive/` is a historical preservation -- never modify those files

## Content Sources

The wiki synthesizes content from:
- [be300-framebuffer](https://github.com/jroark/be300-framebuffer) repo -- CLAUDE.md, docs/, hardware dumps, reverse engineering notes
- linux4.be Wayback Machine archive -- the original project website (2002-2004)
- Real BE-300 hardware captures -- register dumps, TLB snapshots, diagnostic output
