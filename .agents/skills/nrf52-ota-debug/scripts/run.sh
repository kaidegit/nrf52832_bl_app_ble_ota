#!/usr/bin/env bash
set -euo pipefail

ROOT="/Volumes/aigo_1t/DevPkgs/demos/nrf52832_bl_app_ble_ota"
BUILD_DIR="$ROOT/build"
ELF="$BUILD_DIR/nrf52832_s113_app.elf"

cmake --build "$BUILD_DIR" --target full_hex
cmake --build "$BUILD_DIR" --target flash_all
probe-rs attach --chip nrf52832_xxAA "$ELF"
