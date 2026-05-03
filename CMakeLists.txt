cmake_minimum_required(VERSION 3.21)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/nrf_defaults.cmake)

project(nrf52832_bl_app_ble_ota C ASM)

include(ExternalProject)

set(USER_HOME $ENV{HOME})

set(APP_TARGET app)
set(APP_OUTPUT_BASENAME nrf52832_s113_app)
set(APP_ELF ${CMAKE_CURRENT_BINARY_DIR}/${APP_OUTPUT_BASENAME}.elf)
set(APP_HEX ${CMAKE_CURRENT_BINARY_DIR}/${APP_OUTPUT_BASENAME}.hex)
set(APP_BIN ${CMAKE_CURRENT_BINARY_DIR}/${APP_OUTPUT_BASENAME}.bin)
set(APP_MAP ${CMAKE_CURRENT_BINARY_DIR}/${APP_OUTPUT_BASENAME}.map)

set(BOOTLOADER_TARGET secure_bootloader_ble_s113)
set(BOOTLOADER_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/bl)
set(BOOTLOADER_ELF ${BOOTLOADER_BUILD_DIR}/${BOOTLOADER_TARGET}.elf)
set(BOOTLOADER_HEX ${BOOTLOADER_BUILD_DIR}/${BOOTLOADER_TARGET}.hex)
set(BOOTLOADER_BIN ${BOOTLOADER_BUILD_DIR}/${BOOTLOADER_TARGET}.bin)
set(BOOTLOADER_MAP ${BOOTLOADER_BUILD_DIR}/${BOOTLOADER_TARGET}.map)

set(OTA_ZIP ${CMAKE_CURRENT_BINARY_DIR}/nrf52832_s113_app_dfu_v${APP_VERSION}.zip)
set(SETTINGS_HEX ${CMAKE_CURRENT_BINARY_DIR}/nrf52832_bl_settings_v${APP_VERSION}.hex)
set(FULL_HEX ${CMAKE_CURRENT_BINARY_DIR}/nrf52832_s113_full.hex)

find_program(ARM_OBJCOPY arm-none-eabi-objcopy PATHS ${ARM_GCC_PATH} NO_DEFAULT_PATH REQUIRED)
find_program(ARM_SIZE arm-none-eabi-size PATHS ${ARM_GCC_PATH} NO_DEFAULT_PATH REQUIRED)

set(NRFUTIL_EXECUTABLE nrfutil CACHE STRING "nrfutil executable")
set(MERGEHEX_EXECUTABLE mergehex CACHE STRING "mergehex executable")
set(NRFUTIL_HOME ${CMAKE_CURRENT_LIST_DIR}/tools)
set(NRFUTIL_STATE_DIR ${NRFUTIL_HOME}/.nrfutil)
set(NRFUTIL_SEED_DIR ${USER_HOME}/.nrfutil)

set(SDK_INC
    ${CMAKE_CURRENT_LIST_DIR}/config/app
    ${CMAKE_CURRENT_LIST_DIR}/components/nrf_log_wrapper
    ${SDK_PATH}/components
    ${SDK_PATH}/components/ble/ble_advertising
    ${SDK_PATH}/components/ble/ble_link_ctx_manager
    ${SDK_PATH}/components/ble/ble_services/ble_dfu
    ${SDK_PATH}/components/ble/ble_services/ble_nus
    ${SDK_PATH}/components/ble/common
    ${SDK_PATH}/components/ble/nrf_ble_gatt
    ${SDK_PATH}/components/ble/nrf_ble_qwr
    ${SDK_PATH}/components/ble/peer_manager
    ${SDK_PATH}/components/boards
    ${SDK_PATH}/components/libraries/atomic
    ${SDK_PATH}/components/libraries/atomic_fifo
    ${SDK_PATH}/components/libraries/atomic_flags
    ${SDK_PATH}/components/libraries/balloc
    ${SDK_PATH}/components/libraries/bootloader
    ${SDK_PATH}/components/libraries/bootloader/ble_dfu
    ${SDK_PATH}/components/libraries/bootloader/dfu
    ${SDK_PATH}/components/libraries/bsp
    ${SDK_PATH}/components/libraries/button
    ${SDK_PATH}/components/libraries/crc16
    ${SDK_PATH}/components/libraries/delay
    ${SDK_PATH}/components/libraries/experimental_section_vars
    ${SDK_PATH}/components/libraries/hardfault
    ${SDK_PATH}/components/libraries/hardfault/nrf52
    ${SDK_PATH}/components/libraries/log
    ${SDK_PATH}/components/libraries/log/src
    ${SDK_PATH}/components/libraries/memobj
    ${SDK_PATH}/components/libraries/mutex
    ${SDK_PATH}/components/libraries/pwr_mgmt
    ${SDK_PATH}/components/libraries/ringbuf
    ${SDK_PATH}/components/libraries/scheduler
    ${SDK_PATH}/components/libraries/sortlist
    ${SDK_PATH}/components/libraries/strerror
    ${SDK_PATH}/components/libraries/svc
    ${SDK_PATH}/components/libraries/timer
    ${SDK_PATH}/components/libraries/util
    ${SDK_PATH}/components/softdevice/common
    ${SDK_PATH}/components/softdevice/s113/headers
    ${SDK_PATH}/components/softdevice/s113/headers/nrf52
    ${SDK_PATH}/components/toolchain/cmsis/include
    ${SDK_PATH}/external/fprintf
    ${SDK_PATH}/external/freertos/config
    ${SDK_PATH}/external/freertos/portable/CMSIS/nrf52
    ${SDK_PATH}/external/freertos/portable/GCC/nrf52
    ${SDK_PATH}/external/freertos/source/include
    ${SDK_PATH}/external/segger_rtt
    ${SDK_PATH}/external/utf_converter
    ${SDK_PATH}/integration/nrfx
    ${SDK_PATH}/integration/nrfx/legacy
    ${SDK_PATH}/modules/nrfx
    ${SDK_PATH}/modules/nrfx/drivers/include
    ${SDK_PATH}/modules/nrfx/hal
    ${SDK_PATH}/modules/nrfx/mdk
    ${CMAKE_CURRENT_LIST_DIR}/components/elog_port
    ${CMAKE_CURRENT_LIST_DIR}/components/EasyLogger/easylogger/inc
)

set(APP_SOURCES
    ${SDK_PATH}/modules/nrfx/mdk/gcc_startup_nrf52.S
    ${SDK_PATH}/modules/nrfx/mdk/system_nrf52.c
    ${SDK_PATH}/components/libraries/util/app_error.c
    ${SDK_PATH}/components/libraries/util/app_error_handler_gcc.c
    ${SDK_PATH}/components/libraries/util/app_error_weak.c
    ${SDK_PATH}/components/libraries/util/app_util_platform.c
    ${SDK_PATH}/components/libraries/util/nrf_assert.c
    ${SDK_PATH}/components/libraries/crc16/crc16.c
    ${SDK_PATH}/components/libraries/timer/app_timer2.c
    ${SDK_PATH}/components/libraries/timer/drv_rtc.c
    ${SDK_PATH}/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c
    ${SDK_PATH}/components/libraries/hardfault/hardfault_implementation.c
    ${SDK_PATH}/components/libraries/hardfault/nrf52/handler/hardfault_handler_gcc.c
    ${SDK_PATH}/components/libraries/atomic/nrf_atomic.c
    ${SDK_PATH}/components/libraries/atomic_fifo/nrf_atfifo.c
    ${SDK_PATH}/components/libraries/atomic_flags/nrf_atflags.c
    ${SDK_PATH}/components/libraries/balloc/nrf_balloc.c
    ${SDK_PATH}/components/libraries/memobj/nrf_memobj.c
    ${SDK_PATH}/components/libraries/ringbuf/nrf_ringbuf.c
    ${SDK_PATH}/components/libraries/experimental_section_vars/nrf_section_iter.c
    ${SDK_PATH}/components/libraries/sortlist/nrf_sortlist.c
    ${SDK_PATH}/components/libraries/strerror/nrf_strerror.c
    ${SDK_PATH}/external/fprintf/nrf_fprintf.c
    ${SDK_PATH}/external/fprintf/nrf_fprintf_format.c
    ${SDK_PATH}/external/segger_rtt/SEGGER_RTT.c
    ${SDK_PATH}/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c
    ${SDK_PATH}/external/segger_rtt/SEGGER_RTT_printf.c
    ${SDK_PATH}/external/utf_converter/utf.c
    ${CMAKE_CURRENT_LIST_DIR}/components/EasyLogger/easylogger/src/elog.c
    ${CMAKE_CURRENT_LIST_DIR}/components/EasyLogger/easylogger/src/elog_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/components/elog_port/elog_port.c
    ${SDK_PATH}/integration/nrfx/legacy/nrf_drv_clock.c
    ${SDK_PATH}/modules/nrfx/soc/nrfx_atomic.c
    ${SDK_PATH}/modules/nrfx/drivers/src/nrfx_clock.c
    ${SDK_PATH}/modules/nrfx/drivers/src/nrfx_gpiote.c
    ${SDK_PATH}/modules/nrfx/drivers/src/prs/nrfx_prs.c
    ${SDK_PATH}/components/ble/common/ble_advdata.c
    ${SDK_PATH}/components/ble/ble_advertising/ble_advertising.c
    ${SDK_PATH}/components/ble/common/ble_conn_params.c
    ${SDK_PATH}/components/ble/common/ble_conn_state.c
    ${SDK_PATH}/components/ble/common/ble_srv_common.c
    ${SDK_PATH}/components/ble/ble_link_ctx_manager/ble_link_ctx_manager.c
    ${SDK_PATH}/components/ble/nrf_ble_gatt/nrf_ble_gatt.c
    ${SDK_PATH}/components/ble/nrf_ble_qwr/nrf_ble_qwr.c
    ${SDK_PATH}/components/ble/ble_services/ble_nus/ble_nus.c
    ${SDK_PATH}/components/ble/ble_services/ble_dfu/ble_dfu.c
    ${SDK_PATH}/components/ble/ble_services/ble_dfu/ble_dfu_bonded.c
    ${SDK_PATH}/components/ble/ble_services/ble_dfu/ble_dfu_unbonded.c
    ${SDK_PATH}/components/libraries/bootloader/dfu/nrf_dfu_svci.c
    ${SDK_PATH}/components/softdevice/common/nrf_sdh.c
    ${SDK_PATH}/components/softdevice/common/nrf_sdh_ble.c
    ${CMAKE_CURRENT_LIST_DIR}/src/patch/nrf_sdh_freertos.c
    ${SDK_PATH}/components/softdevice/common/nrf_sdh_soc.c
    ${SDK_PATH}/external/freertos/source/croutine.c
    ${SDK_PATH}/external/freertos/source/event_groups.c
    ${SDK_PATH}/external/freertos/source/list.c
    ${SDK_PATH}/external/freertos/source/portable/MemMang/heap_4.c
    ${SDK_PATH}/external/freertos/portable/GCC/nrf52/port.c
    ${SDK_PATH}/external/freertos/portable/CMSIS/nrf52/port_cmsis.c
    ${SDK_PATH}/external/freertos/portable/CMSIS/nrf52/port_cmsis_systick.c
    ${SDK_PATH}/external/freertos/source/queue.c
    ${SDK_PATH}/external/freertos/source/stream_buffer.c
    ${SDK_PATH}/external/freertos/source/tasks.c
    ${SDK_PATH}/external/freertos/source/timers.c
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)

add_executable(${APP_TARGET} ${APP_SOURCES})

set_source_files_properties(
    ${SDK_PATH}/external/segger_rtt/SEGGER_RTT.c
    PROPERTIES COMPILE_OPTIONS "-fno-data-sections"
)

set_target_properties(${APP_TARGET} PROPERTIES
    OUTPUT_NAME ${APP_OUTPUT_BASENAME}
    SUFFIX .elf
)

target_include_directories(${APP_TARGET} PRIVATE ${SDK_INC})

target_compile_definitions(${APP_TARGET} PRIVATE
    PROJECT_IS_APP
    APP_TIMER_V2
    APP_TIMER_V2_RTC1_ENABLED
    BL_SETTINGS_ACCESS_ONLY
    CONFIG_GPIO_AS_PINRESET
    FLOAT_ABI_HARD
    FREERTOS
    NRF52
    NRF52832_XXAA
    NRF52_PAN_74
    NRF_DFU_SVCI_ENABLED
    NRF_DFU_TRANSPORT_BLE=1
    NRF_SD_BLE_API_VERSION=7
    NRF_BLE_FREERTOS_SDH_TASK_STACK=512
    SEGGER_RTT_SECTION=\".rtt_shared\"
    SEGGER_RTT_BUFFER_SECTION=\".rtt_shared\"
    S113
    SOFTDEVICE_PRESENT
    __HEAP_SIZE=8192
    __STACK_SIZE=4096
)

target_compile_options(${APP_TARGET} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mabi=aapcs
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -ffunction-sections
    -fdata-sections
    -fno-strict-aliasing
    -fno-builtin
    -fshort-enums
    -Wall
    -Wno-array-bounds
    -Wno-unused-function
    -O2
    -g3
)

target_link_options(${APP_TARGET} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mabi=aapcs
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -L${SDK_PATH}/modules/nrfx/mdk
    -T${CMAKE_CURRENT_LIST_DIR}/config/app/app_gcc_nrf52.ld
    -Wl,--gc-sections
    -Wl,-Map=${APP_MAP}
    -Wl,--print-memory-usage
    --specs=nano.specs
)

target_link_libraries(${APP_TARGET} PRIVATE c nosys m)

add_custom_command(TARGET ${APP_TARGET} POST_BUILD
    COMMAND ${ARM_OBJCOPY} -O ihex $<TARGET_FILE:${APP_TARGET}> ${APP_HEX}
    COMMAND ${ARM_OBJCOPY} -O binary $<TARGET_FILE:${APP_TARGET}> ${APP_BIN}
    COMMAND ${ARM_SIZE} $<TARGET_FILE:${APP_TARGET}>
    BYPRODUCTS ${APP_HEX} ${APP_BIN} ${APP_MAP}
    VERBATIM
)

cmake_host_system_information(RESULT NPROC QUERY NUMBER_OF_LOGICAL_CORES)

ExternalProject_Add(bootloader_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/bl
    BINARY_DIR ${BOOTLOADER_BUILD_DIR}
    CMAKE_ARGS
        -G ${CMAKE_GENERATOR}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DARM_GCC_PATH=${ARM_GCC_PATH}
        -DSDK_PATH=${SDK_PATH}
        -DSOFTDEVICE_PATH=${SOFTDEVICE_PATH}
        -DSOFTDEVICE_HEX=${SOFTDEVICE_HEX}
        -DAPP_VERSION=${APP_VERSION}
        -DBOOTLOADER_VERSION=${BOOTLOADER_VERSION}
    BUILD_BYPRODUCTS
        ${BOOTLOADER_ELF}
        ${BOOTLOADER_HEX}
        ${BOOTLOADER_BIN}
        ${BOOTLOADER_MAP}
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --target ${BOOTLOADER_TARGET} -- -j${NPROC}
    INSTALL_COMMAND ""
    BUILD_ALWAYS 1
    USES_TERMINAL_BUILD 1
)

add_custom_target(bl DEPENDS bootloader_project)

add_custom_target(ota_zip
    DEPENDS ${APP_TARGET}
    COMMAND ${CMAKE_COMMAND} -E chdir ${NRFUTIL_HOME} sh -c "pwd; ls -la .nrfutil 2>/dev/null || echo '.nrfutil not found'; test -e .nrfutil || ln -sv ${NRFUTIL_SEED_DIR} .nrfutil"
    COMMAND ${CMAKE_COMMAND} -E env HOME=${NRFUTIL_HOME} ${NRFUTIL_EXECUTABLE} nrf5sdk-tools pkg generate
        --hw-version 52
        --sd-req 0x102
        --application ${APP_HEX}
        --application-version ${APP_VERSION}
        --app-boot-validation VALIDATE_GENERATED_CRC
        ${OTA_ZIP}
    BYPRODUCTS ${OTA_ZIP}
    VERBATIM
)

add_custom_target(settings_hex
    DEPENDS ${APP_TARGET}
    COMMAND ${CMAKE_COMMAND} -E chdir ${NRFUTIL_HOME} sh -c "pwd; ls -la .nrfutil 2>/dev/null || echo '.nrfutil not found'; test -e .nrfutil || ln -sv ${NRFUTIL_SEED_DIR} .nrfutil"
    COMMAND ${CMAKE_COMMAND} -E env HOME=${NRFUTIL_HOME} ${NRFUTIL_EXECUTABLE} nrf5sdk-tools settings generate
        --family NRF52
        --application ${APP_HEX}
        --application-version ${APP_VERSION}
        --bootloader-version ${BOOTLOADER_VERSION}
        --bl-settings-version 2
        --app-boot-validation VALIDATE_GENERATED_CRC
        ${SETTINGS_HEX}
    BYPRODUCTS ${SETTINGS_HEX}
    VERBATIM
)

add_custom_target(full_hex
    DEPENDS ${APP_TARGET} bl settings_hex
    COMMAND ${MERGEHEX_EXECUTABLE}
        --merge
        ${SOFTDEVICE_HEX}
        ${BOOTLOADER_HEX}
        ${SETTINGS_HEX}
        ${APP_HEX}
        --output ${FULL_HEX}
    BYPRODUCTS ${FULL_HEX}
    VERBATIM
)

add_custom_target(flash_all
    COMMAND probe-rs download --chip nrf52832_xxAA --binary-format hex ${FULL_HEX}
    COMMENT "Flashing ${FULL_HEX} to nrf52832"
)
