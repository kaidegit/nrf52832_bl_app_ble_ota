set(ARM_GCC_PATH "/Users/kai/DevTools/arm-gnu-toolchain-13.3.rel1-darwin-arm64-arm-none-eabi/bin" CACHE PATH "ARM GCC bin directory")
set(SDK_PATH "/Volumes/aigo_1t/DevPkgs/nordic/nRF5_SDK_17.1.0_ddde560" CACHE PATH "Nordic nRF5 SDK root")
set(SOFTDEVICE_PATH "/Volumes/aigo_1t/DevPkgs/nordic/s113nrf52720" CACHE PATH "SoftDevice root")
set(SOFTDEVICE_HEX "${SOFTDEVICE_PATH}/s113_nrf52_7.2.0_softdevice.hex" CACHE FILEPATH "SoftDevice hex")

set(APP_FLASH_ORIGIN 0x1C000 CACHE STRING "Application flash origin")
set(APP_FLASH_LENGTH 0x5C000 CACHE STRING "Application flash length")
set(APP_RAM_ORIGIN 0x20003000 CACHE STRING "Application RAM origin")
set(APP_RAM_LENGTH 0xD000 CACHE STRING "Application RAM length")
set(BOOTLOADER_SETTINGS_PAGE 0x7F000 CACHE STRING "Bootloader settings page")
set(APP_VERSION 1 CACHE STRING "Application version")
set(BOOTLOADER_VERSION 1 CACHE STRING "Bootloader version")

if(NOT DEFINED CMAKE_SYSTEM_NAME)
    set(CMAKE_SYSTEM_NAME Generic)
endif()

if(NOT DEFINED CMAKE_SYSTEM_PROCESSOR)
    set(CMAKE_SYSTEM_PROCESSOR arm)
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(ARM_GCC_PREFIX ${ARM_GCC_PATH}/arm-none-eabi)

if(NOT DEFINED CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER ${ARM_GCC_PREFIX}-gcc CACHE FILEPATH "C compiler" FORCE)
endif()

if(NOT DEFINED CMAKE_ASM_COMPILER)
    set(CMAKE_ASM_COMPILER ${ARM_GCC_PREFIX}-gcc CACHE FILEPATH "ASM compiler" FORCE)
endif()

if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER ${ARM_GCC_PREFIX}-g++ CACHE FILEPATH "CXX compiler" FORCE)
endif()
