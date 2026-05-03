# Web And Android Frontends

The emulator core is shared by three frontends:

- native CLI/SDL (`build-host/be300`)
- browser/WebAssembly (`build-web/web/`)
- Android JNI app (`android/app/`)

## WebAssembly

Build from the emulator repo root:

```bash
./tools/build_web.sh
```

The script configures an Emscripten build with `BUILD_WEB=ON` and writes the
bundle to `build-web/web/`.

Generated outputs include:

- `index.html`, `app.js`, `worker.js`, `styles.css`
- `be300_web.js`, `be300_web.wasm`
- frame/button assets copied from `src/ui/assets/`

Run locally by serving the output directory:

```bash
cp ce/restore_images/All_nand_300.bin build-web/web/
python3 -m http.server --directory build-web/web 8000
```

Open `http://127.0.0.1:8000` and click Boot. If the NAND image is not bundled
next to the page, upload one through the controls.

### Web Runtime Notes

- Web boots initialize the guest RTC from the browser host's local date/time by
  default, equivalent to native `--rtc-host-time`.
- The web path cold-boots WinCE from NAND; it does not provide the historical
  Linux boot path.
- The worker drives the core through exported functions such as
  `_be300_step`, `_be300_copy_frame_rgba8888`, `_be300_set_touch`,
  `_be300_set_buttons`, and `_be300_destroy`.
- Optional CF and NE2000 accessories are staged before machine creation.
- Browser NE2000 can use the internal GXemul IPv4 gateway or a WebSocket
  bridge where each binary message is one Ethernet frame.

## Android

Build from the emulator repo:

```bash
cd android
./gradlew :app:assembleDebug
```

The debug APK is written to:

```text
android/app/build/outputs/apk/debug/app-debug.apk
```

The app embeds the same emulator core through JNI, renders the 240x320
framebuffer inside the BE-300 frame asset, and forwards touch, frame buttons,
and hardware keyboard events to the guest.

### Android NAND Images

The app can boot a user-selected NAND image from Android storage. For a local
debug build, `All_nand_300.bin` can be placed in `app/src/main/assets/`; the
app copies it into private storage on launch.

### Hardware Keyboard

Android hardware keyboard events are mapped to Stowaway dock scancodes and
sent through the same `stowaway_queue_key` path used by the native and web
frontends. A NAND image with the Stowaway driver registered is required for
guest keystrokes to reach WinCE controls.
