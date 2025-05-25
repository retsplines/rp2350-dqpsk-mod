# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_EXTRAS_FETCH_FROM_GIT ON)

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
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# We also need PICO EXTRAS
include(pico_extras_import.cmake)

project(dqpskmod C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(dqpskmod main.c modulator.c filter.c)

pico_generate_pio_header(dqpskmod ${CMAKE_CURRENT_LIST_DIR}/i2s.pio)

pico_set_program_name(dqpskmod "dqpskmod")
pico_set_program_version(dqpskmod "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(dqpskmod 1)
pico_enable_stdio_usb(dqpskmod 0)

# Add the standard library to the build
target_link_libraries(dqpskmod
        pico_stdlib
        pico_rand)

# Add the standard include files to the build
target_include_directories(dqpskmod PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(dqpskmod 
        pico_audio_i2s
	    pico_rand
        hardware_timer
        hardware_spi
        hardware_clocks
)

pico_add_extra_outputs(dqpskmod)
