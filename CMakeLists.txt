# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pimoroni_pico_plus2_rp2350 CACHE STRING "Board type")
set(PICOCALC_LOGO_VERSION "1.0.0-alpha.1" CACHE STRING "PicoCalc Logo version")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(picocalc-logo C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(picocalc-logo
        main.c
        picocalc/picocalc.c
        picocalc/picocalc.h
        picocalc/screen.c
        picocalc/screen.h
        evaluate.c
        evaluate.h
        input.c
        input.h
        license.c
        turtle.c
        turtle.h
        modules/picocalc-text-starter/drivers/audio.c
        modules/picocalc-text-starter/drivers/audio.h
        modules/picocalc-text-starter/drivers/clib.c
        modules/picocalc-text-starter/drivers/fat32.c
        modules/picocalc-text-starter/drivers/fat32.h
        modules/picocalc-text-starter/drivers/font-5x10.c
        modules/picocalc-text-starter/drivers/font-8x10.c
        modules/picocalc-text-starter/drivers/font.h
        modules/picocalc-text-starter/drivers/keyboard.c
        modules/picocalc-text-starter/drivers/keyboard.h
        modules/picocalc-text-starter/drivers/lcd.c
        modules/picocalc-text-starter/drivers/lcd.h
        modules/picocalc-text-starter/drivers/sdcard.c
        modules/picocalc-text-starter/drivers/sdcard.h
        modules/picocalc-text-starter/drivers/southbridge.c
        modules/picocalc-text-starter/drivers/southbridge.h
        )

pico_set_program_name(picocalc-logo "picocalc-logo")
pico_set_program_url(picocalc-logo "https://github.com/BlairLeduc/picocalc-logo")
pico_set_program_version(picocalc-logo "${PICOCALC_LOGO_VERSION}")

# Generate PIO header
pico_generate_pio_header(picocalc-logo ${CMAKE_CURRENT_LIST_DIR}/modules/picocalc-text-starter/drivers/audio.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(picocalc-logo 0)
pico_enable_stdio_usb(picocalc-logo 0)

# Turn on all warnings
target_compile_options(picocalc-logo PRIVATE -Wall -Werror)

# Add the standard library to the build
target_link_libraries(picocalc-logo
        pico_stdlib)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    @ONLY
)
# Add the standard include files to the build
target_include_directories(picocalc-logo PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/modules/picocalc-text-starter
)

# Add any user requested libraries
target_link_libraries(picocalc-logo 
        pico_stdlib
        pico_printf
        pico_float
        pico_rand
        hardware_gpio
        hardware_i2c
        hardware_spi
        hardware_pio
        hardware_clocks
        )

# Add wireless library only for boards that support it
if(PICO_BOARD STREQUAL "pico_w" 
        OR PICO_BOARD STREQUAL "pico2_w"
        OR PICO_BOARD STREQUAL "pimoroni_pico_plus2_w_rp2350")
    target_link_libraries(picocalc-text-starter
            pico_cyw43_arch_none
            )
endif()

pico_add_extra_outputs(picocalc-logo)

