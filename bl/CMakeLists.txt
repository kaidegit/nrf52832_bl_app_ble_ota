cmake_minimum_required(VERSION 3.21)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/nrf_defaults.cmake)

project(secure_bootloader_ble_s113 C ASM)

set(BOOTLOADER_TARGET secure_bootloader_ble_s113)
set(BOOTLOADER_OUTPUT_BASENAME ${BOOTLOADER_TARGET})
set(BOOTLOADER_ELF ${CMAKE_CURRENT_BINARY_DIR}/${BOOTLOADER_OUTPUT_BASENAME}.elf)
set(BOOTLOADER_HEX ${CMAKE_CURRENT_BINARY_DIR}/${BOOTLOADER_OUTPUT_BASENAME}.hex)
set(BOOTLOADER_BIN ${CMAKE_CURRENT_BINARY_DIR}/${BOOTLOADER_OUTPUT_BASENAME}.bin)
set(BOOTLOADER_MAP ${CMAKE_CURRENT_BINARY_DIR}/${BOOTLOADER_OUTPUT_BASENAME}.map)

find_program(ARM_OBJCOPY arm-none-eabi-objcopy PATHS ${ARM_GCC_PATH} NO_DEFAULT_PATH REQUIRED)
find_program(ARM_SIZE arm-none-eabi-size PATHS ${ARM_GCC_PATH} NO_DEFAULT_PATH REQUIRED)

set(SDK_INC
    ${CMAKE_CURRENT_LIST_DIR}/../config/bl
    ${CMAKE_CURRENT_LIST_DIR}
    ${SDK_PATH}/components
    ${SDK_PATH}/components/ble/ble_services/ble_dfu
    ${SDK_PATH}/components/ble/common
    ${SDK_PATH}/components/boards
    ${SDK_PATH}/components/libraries/atomic
    ${SDK_PATH}/components/libraries/atomic_fifo
    ${SDK_PATH}/components/libraries/balloc
    ${SDK_PATH}/components/libraries/bootloader
    ${SDK_PATH}/components/libraries/bootloader/ble_dfu
    ${SDK_PATH}/components/libraries/bootloader/dfu
    ${SDK_PATH}/components/libraries/crc32
    ${SDK_PATH}/components/libraries/crypto
    ${SDK_PATH}/components/libraries/crypto/backend/cc310
    ${SDK_PATH}/components/libraries/crypto/backend/cc310_bl
    ${SDK_PATH}/components/libraries/crypto/backend/cifra
    ${SDK_PATH}/components/libraries/crypto/backend/mbedtls
    ${SDK_PATH}/components/libraries/crypto/backend/micro_ecc
    ${SDK_PATH}/components/libraries/crypto/backend/nrf_hw
    ${SDK_PATH}/components/libraries/crypto/backend/nrf_sw
    ${SDK_PATH}/components/libraries/crypto/backend/oberon
    ${SDK_PATH}/components/libraries/crypto/backend/optiga
    ${SDK_PATH}/components/libraries/delay
    ${SDK_PATH}/components/libraries/experimental_section_vars
    ${SDK_PATH}/components/libraries/fstorage
    ${SDK_PATH}/components/libraries/log
    ${SDK_PATH}/components/libraries/log/src
    ${SDK_PATH}/components/libraries/mem_manager
    ${SDK_PATH}/components/libraries/memobj
    ${SDK_PATH}/components/libraries/queue
    ${SDK_PATH}/components/libraries/ringbuf
    ${SDK_PATH}/components/libraries/scheduler
    ${SDK_PATH}/components/libraries/sha256
    ${SDK_PATH}/components/libraries/strerror
    ${SDK_PATH}/components/libraries/stack_info
    ${SDK_PATH}/components/libraries/svc
    ${SDK_PATH}/components/libraries/util
    ${SDK_PATH}/components/softdevice/common
    ${SDK_PATH}/components/softdevice/s113/headers
    ${SDK_PATH}/components/softdevice/s113/headers/nrf52
    ${SDK_PATH}/components/toolchain/cmsis/include
    ${SDK_PATH}/external/nano-pb
    ${SDK_PATH}/external/nrf_oberon
    ${SDK_PATH}/external/nrf_oberon/include
    ${SDK_PATH}/integration/nrfx
    ${SDK_PATH}/modules/nrfx
    ${SDK_PATH}/modules/nrfx/hal
    ${SDK_PATH}/modules/nrfx/mdk
)

set(BOOTLOADER_SOURCES
    ${SDK_PATH}/modules/nrfx/mdk/gcc_startup_nrf52.S
    ${SDK_PATH}/modules/nrfx/mdk/system_nrf52.c
    ${SDK_PATH}/components/libraries/util/app_error_weak.c
    ${SDK_PATH}/components/libraries/scheduler/app_scheduler.c
    ${SDK_PATH}/components/libraries/util/app_util_platform.c
    ${SDK_PATH}/components/libraries/crc32/crc32.c
    ${SDK_PATH}/components/libraries/mem_manager/mem_manager.c
    ${SDK_PATH}/components/libraries/util/nrf_assert.c
    ${SDK_PATH}/components/libraries/atomic_fifo/nrf_atfifo.c
    ${SDK_PATH}/components/libraries/atomic/nrf_atomic.c
    ${SDK_PATH}/components/libraries/balloc/nrf_balloc.c
    ${SDK_PATH}/components/libraries/fstorage/nrf_fstorage.c
    ${SDK_PATH}/components/libraries/fstorage/nrf_fstorage_nvmc.c
    ${SDK_PATH}/components/libraries/fstorage/nrf_fstorage_sd.c
    ${SDK_PATH}/components/libraries/queue/nrf_queue.c
    ${SDK_PATH}/components/libraries/ringbuf/nrf_ringbuf.c
    ${SDK_PATH}/components/libraries/experimental_section_vars/nrf_section_iter.c
    ${SDK_PATH}/components/libraries/strerror/nrf_strerror.c
    ${SDK_PATH}/components/libraries/sha256/sha256.c
    ${SDK_PATH}/modules/nrfx/hal/nrf_nvmc.c
    ${SDK_PATH}/modules/nrfx/soc/nrfx_atomic.c
    ${SDK_PATH}/components/libraries/crypto/nrf_crypto_ecc.c
    ${SDK_PATH}/components/libraries/crypto/nrf_crypto_ecdsa.c
    ${SDK_PATH}/components/libraries/crypto/nrf_crypto_hash.c
    ${SDK_PATH}/components/libraries/crypto/nrf_crypto_init.c
    ${SDK_PATH}/components/libraries/crypto/nrf_crypto_shared.c
    ${CMAKE_CURRENT_LIST_DIR}/dfu_public_key.c
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${SDK_PATH}/components/ble/common/ble_srv_common.c
    ${SDK_PATH}/components/libraries/bootloader/nrf_bootloader.c
    ${SDK_PATH}/components/libraries/bootloader/nrf_bootloader_app_start.c
    ${SDK_PATH}/components/libraries/bootloader/nrf_bootloader_app_start_final.c
    ${SDK_PATH}/components/libraries/bootloader/nrf_bootloader_dfu_timers.c
    ${SDK_PATH}/components/libraries/bootloader/nrf_bootloader_fw_activation.c
    ${SDK_PATH}/components/libraries/bootloader/nrf_bootloader_info.c
    ${SDK_PATH}/components/libraries/bootloader/nrf_bootloader_wdt.c
    ${SDK_PATH}/external/nano-pb/pb_common.c
    ${SDK_PATH}/external/nano-pb/pb_decode.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/dfu-cc.pb.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu.c
    ${SDK_PATH}/components/libraries/bootloader/ble_dfu/nrf_dfu_ble.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_flash.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_handling_error.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_mbr.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_req_handler.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_settings.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_settings_svci.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_transport.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_utils.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_validation.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_ver_validation.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_svci.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_svci_handler.c
    ${SDK_PATH}/components/libraries/svc/nrf_svc_handler.c
    ${SDK_PATH}/components/softdevice/common/nrf_sdh.c
    ${SDK_PATH}/components/softdevice/common/nrf_sdh_ble.c
    ${SDK_PATH}/components/softdevice/common/nrf_sdh_soc.c
    ${SDK_PATH}/components/libraries/crypto/backend/oberon/oberon_backend_ecc.c
    ${SDK_PATH}/components/libraries/crypto/backend/oberon/oberon_backend_ecdsa.c
    ${SDK_PATH}/components/libraries/crypto/backend/oberon/oberon_backend_hash.c
)

add_executable(${BOOTLOADER_TARGET} ${BOOTLOADER_SOURCES})

set_target_properties(${BOOTLOADER_TARGET} PROPERTIES
    OUTPUT_NAME ${BOOTLOADER_OUTPUT_BASENAME}
    SUFFIX .elf
)

target_include_directories(${BOOTLOADER_TARGET} PRIVATE ${SDK_INC})

target_compile_definitions(${BOOTLOADER_TARGET} PRIVATE
    BLE_STACK_SUPPORT_REQD
    BOARD_PCA10040
    CONFIG_GPIO_AS_PINRESET
    FLOAT_ABI_HARD
    NRF52
    NRF52832_XXAA
    NRF52_PAN_74
    NRF_DFU_SETTINGS_VERSION=2
    NRF_DFU_SVCI_ENABLED
    NRF_SD_BLE_API_VERSION=7
    S113
    SOFTDEVICE_PRESENT
    SVC_INTERFACE_CALL_AS_NORMAL_FUNCTION
    BOOTLOADER_SIZE=0x8000
    __START=main
    __STARTUP_CLEAR_BSS
    __HEAP_SIZE=0
    __STACK_SIZE=2048
)

target_compile_options(${BOOTLOADER_TARGET} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mabi=aapcs
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -ffunction-sections
    -fdata-sections
    -flto
    -fno-strict-aliasing
    -fno-builtin
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -fshort-enums
    -Wall
    -Werror
    -Wno-array-bounds
    -Os
    -g3
)

target_link_options(${BOOTLOADER_TARGET} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mabi=aapcs
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -nostartfiles
    -flto
    -L${SDK_PATH}/modules/nrfx/mdk
    -T${CMAKE_CURRENT_LIST_DIR}/../config/bl/secure_bootloader_gcc_nrf52.ld
    -Wl,--gc-sections
    -Wl,-Map=${BOOTLOADER_MAP}
    -Wl,--print-memory-usage
    --specs=nano.specs
)

target_link_libraries(${BOOTLOADER_TARGET} PRIVATE
    ${SDK_PATH}/external/nrf_oberon/lib/cortex-m4/hard-float/liboberon_3.0.8.a
    c
    nosys
    m
)

add_custom_command(TARGET ${BOOTLOADER_TARGET} POST_BUILD
    COMMAND ${ARM_OBJCOPY} -O ihex $<TARGET_FILE:${BOOTLOADER_TARGET}> ${BOOTLOADER_HEX}
    COMMAND ${ARM_OBJCOPY}
        -O binary
        -R .uicr_bootloader_start_address
        -R .uicr_mbr_params_page
        -R .stack
        -R .heap
        $<TARGET_FILE:${BOOTLOADER_TARGET}>
        ${BOOTLOADER_BIN}
    COMMAND ${ARM_SIZE} $<TARGET_FILE:${BOOTLOADER_TARGET}>
    BYPRODUCTS ${BOOTLOADER_HEX} ${BOOTLOADER_BIN} ${BOOTLOADER_MAP}
    VERBATIM
)
