---
name: nrf52-ota-debug
description: Build, flash, and live-attach for this nRF52832 OTA demo. Use when diagnosing runtime crashes by running CMake target full_hex, CMake target flash_all, then probe-rs attach to the app ELF and collecting logs/backtraces.
---

# nrf52-ota-debug

Run the exact debug loop for this project in order:

1. Build full image:
`cmake --build build --target full_hex`

2. Flash all images:
`cmake --build build --target flash_all`

3. Attach and collect runtime logs:
`probe-rs attach --chip nrf52832_xxAA /Volumes/aigo_1t/DevPkgs/demos/nrf52832_bl_app_ble_ota/build/nrf52832_s113_app.elf`

If `probe-rs attach` is interactive, keep it running long enough to capture crash output, then stop with Ctrl+C.

Prefer using the helper script at `scripts/run.sh` for repeatable execution.
