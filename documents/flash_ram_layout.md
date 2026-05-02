# nRF52832 S113 BL + App Flash / RAM 分区

## 基本信息

- 芯片：`nRF52832_xxAA`
- Flash 总容量：`512 KB`，地址范围 `0x00000000 - 0x0007FFFF`
- RAM 总容量：`64 KB`，地址范围 `0x20000000 - 0x2000FFFF`
- SoftDevice：`S113 v7.2.0`

## Flash 分区

### 固定分区

| 区域 | 起始地址 | 结束地址 | 大小 | 说明 |
|---|---:|---:|---:|---|
| MBR | `0x00000000` | `0x00000FFF` | `0x1000` / 4 KB | Nordic MBR |
| SoftDevice S113 | `0x00001000` | `0x0001BFFF` | `0x1B000` / 108 KB | 由 App 起始地址反推 |
| App | `0x0001C000` | `0x0006BFFF` | `0x50000` / 320 KB | [config/app/app_gcc_nrf52.ld](/Volumes/aigo_1t/DevPkgs/demos/nrf52832_bl_app_ble_ota/config/app/app_gcc_nrf52.ld:8) |
| Bootloader | `0x0006C000` | `0x00077FFF` | `0xC000` / 48 KB | [config/bl/secure_bootloader_gcc_nrf52.ld](/Volumes/aigo_1t/DevPkgs/demos/nrf52832_bl_app_ble_ota/config/bl/secure_bootloader_gcc_nrf52.ld:8) |
| MBR Params Page | `0x0007E000` | `0x0007EFFF` | `0x1000` / 4 KB | Bootloader / MBR 参数页 |
| Bootloader Settings | `0x0007F000` | `0x0007FFFF` | `0x1000` / 4 KB | DFU settings 页 |

### UICR 相关

| 区域 | 地址 | 大小 | 说明 |
|---|---:|---:|---|
| `UICR.BOOTLOADERADDR` | `0x10001014` | `0x4` | Bootloader 起始地址 |
| `UICR.MBRPARAMADDR` | `0x10001018` | `0x4` | MBR 参数页地址 |

## RAM 分区

### App RAM

| 区域 | 起始地址 | 结束地址 | 大小 | 说明 |
|---|---:|---:|---:|---|
| App RAM | `0x20003000` | `0x2000FBFF` | `0xCC00` / 51 KB | [config/app/app_gcc_nrf52.ld](/Volumes/aigo_1t/DevPkgs/demos/nrf52832_bl_app_ble_ota/config/app/app_gcc_nrf52.ld:9) |
| RTT Shared | `0x2000FC00` | `0x2000FFFF` | `0x400` / 1 KB | [config/app/app_gcc_nrf52.ld](/Volumes/aigo_1t/DevPkgs/demos/nrf52832_bl_app_ble_ota/config/app/app_gcc_nrf52.ld:10) |

说明：

- `0x20000000 - 0x20002FFF` 预留给 `SoftDevice + 协议栈`。
- App 当前使用 FreeRTOS，链接区里已经包含任务栈、静态数据和堆空间预算。
- `0x2000FC00 - 0x2000FFFF` 为 RTT Shared 内存区，App 与 Bootloader 共用同一地址，用于调试通道共享。

### Bootloader RAM

| 区域 | 起始地址 | 结束地址 | 大小 | 说明 |
|---|---:|---:|---:|---|
| Bootloader RAM | `0x20002608` | `0x2000FBFF` | `0xD5F8` / 54,968 bytes | [config/bl/secure_bootloader_gcc_nrf52.ld](/Volumes/aigo_1t/DevPkgs/demos/nrf52832_bl_app_ble_ota/config/bl/secure_bootloader_gcc_nrf52.ld:9) |
| RTT Shared | `0x2000FC00` | `0x2000FFFF` | `0x400` / 1 KB | [config/bl/secure_bootloader_gcc_nrf52.ld](/Volumes/aigo_1t/DevPkgs/demos/nrf52832_bl_app_ble_ota/config/bl/secure_bootloader_gcc_nrf52.ld:10) |

说明：

- `0x20000000 - 0x20002607` 由 `SoftDevice` 占用。
- Bootloader RAM 起点低于 App RAM 起点，这是正常的；两者分别按各自场景单独链接。
- `0x2000FC00 - 0x2000FFFF` 为 RTT Shared 内存区，App 与 Bootloader 共用同一地址，用于调试通道共享。

## 当前构建产物的实际占用

### App

- 链接区域：`Flash 0x1C000 - 0x6BFFF`，总配额 `0x50000`
- 当前镜像大小：
  - `text = 30464`
  - `data = 72`
  - `bss = 15664`
  - `dec = 46200`
  - `hex = 0xB478`

### Bootloader

- 链接区域：`Flash 0x6C000 - 0x77FFF`，总配额 `0xC000`
- 当前镜像大小：
  - `text = 24384`
  - `data = 68`
  - `bss = 17828`
  - `dec = 42280`
  - `hex = 0xA528`

## 合并镜像布局

`full_hex` 由以下内容合并得到：

1. `s113_nrf52_7.2.0_softdevice.hex`
2. `secure_bootloader_ble_s113.hex`
3. `nrf52832_bl_settings_v1.hex`
4. `nrf52832_s113_app.hex`

合并输出：

- `build/nrf52832_s113_full.hex`

## 备注

- App OTA 包当前是 `app-only` 更新，不包含 SoftDevice 和 Bootloader 升级。
- Boot validation 使用 `CRC`。
- 当前 Bootloader 允许 `unsigned application update`，适合开发阶段，不适合量产安全要求场景。
