# Top-level cmake file for building MicroPython on ESP32.

cmake_minimum_required(VERSION 3.12)

# Set the location of this port's directory.
set(MICROPY_PORT_DIR ${CMAKE_SOURCE_DIR})

# Set the board if it's not already set.
if(NOT MICROPY_BOARD)
    set(MICROPY_BOARD GENERIC)
endif()

# Set the board directory and check that it exists.
if(NOT MICROPY_BOARD_DIR)
    set(MICROPY_BOARD_DIR ${MICROPY_PORT_DIR}/boards/${MICROPY_BOARD})
endif()
if(NOT MICROPY_ARDUINO_ESP32_DIR)
    set(MICROPY_ARDUINO_ESP32_DIR ${MICROPY_PORT_DIR}/arduino_esp32)
endif()
if(NOT MICROPY_ARDUINO_MPY_MIXUP_DIR)
    set(MICROPY_ARDUINO_MPY_MIXUP_DIR ${MICROPY_PORT_DIR}/arduino_mpy_mixup)
endif()
if(NOT MICROPY_ARDUINO_LIB_ADAFRUIT_BUS_IO_DIR)
    set(MICROPY_ARDUINO_LIB_ADAFRUIT_BUS_IO_DIR ${MICROPY_PORT_DIR}/arduino_libs/Adafruit_BusIO)
endif()
if(NOT MICROPY_ARDUINO_LIB_ADAFRUIT_GFX_DIR)
    set(MICROPY_ARDUINO_LIB_ADAFRUIT_GFX_DIR ${MICROPY_PORT_DIR}/arduino_libs/Adafruit_GFX)
endif()
if(NOT MICROPY_ARDUINO_LIB_ADAFRUIT_SHARP_DIR)
    set(MICROPY_ARDUINO_LIB_ADAFRUIT_SHARP_DIR ${MICROPY_PORT_DIR}/arduino_libs/Adafruit_SHARP_Memory_Display)
endif()
if(NOT MICROPY_ARDUINO_LIB_RTCLIB_DIR)
    set(MICROPY_ARDUINO_LIB_RTCLIB_DIR ${MICROPY_PORT_DIR}/arduino_libs/RTClib)
endif()
if(NOT EXISTS ${MICROPY_BOARD_DIR}/mpconfigboard.cmake)
    message(FATAL_ERROR "Invalid MICROPY_BOARD specified: ${MICROPY_BOARD}")
endif()

# Include main IDF cmake file.
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Define the output sdkconfig so it goes in the build directory.
set(SDKCONFIG ${CMAKE_BINARY_DIR}/sdkconfig)

# Save the manifest file set from the cmake command line.
set(MICROPY_USER_FROZEN_MANIFEST ${MICROPY_FROZEN_MANIFEST})

# Include board config; this is expected to set SDKCONFIG_DEFAULTS (among other options).
include(${MICROPY_BOARD_DIR}/mpconfigboard.cmake)

# Set the frozen manifest file. Note if MICROPY_FROZEN_MANIFEST is set from the cmake
# command line, then it will override the default and any manifest set by the board.
if (MICROPY_USER_FROZEN_MANIFEST)
    set(MICROPY_FROZEN_MANIFEST ${MICROPY_USER_FROZEN_MANIFEST})
elseif (NOT MICROPY_FROZEN_MANIFEST)
    set(MICROPY_FROZEN_MANIFEST ${MICROPY_PORT_DIR}/boards/manifest.py)
endif()

# Add sdkconfig fragments that depend on the IDF version.
if(IDF_VERSION_MAJOR EQUAL 4 AND IDF_VERSION_MINOR LESS 2)
    set(SDKCONFIG_DEFAULTS ${SDKCONFIG_DEFAULTS} boards/sdkconfig.nimble_core0)
else()
    set(SDKCONFIG_DEFAULTS ${SDKCONFIG_DEFAULTS} boards/sdkconfig.nimble_core1)
endif()

# Concatenate all sdkconfig files into a combined one for the IDF to use.
file(WRITE ${CMAKE_BINARY_DIR}/sdkconfig.combined.in "")
foreach(SDKCONFIG_DEFAULT ${SDKCONFIG_DEFAULTS})
    file(READ ${SDKCONFIG_DEFAULT} CONTENTS)
    file(APPEND ${CMAKE_BINARY_DIR}/sdkconfig.combined.in "${CONTENTS}")
endforeach()
configure_file(${CMAKE_BINARY_DIR}/sdkconfig.combined.in ${CMAKE_BINARY_DIR}/sdkconfig.combined COPYONLY)
set(SDKCONFIG_DEFAULTS ${CMAKE_BINARY_DIR}/sdkconfig.combined)

# Define the project.
project(micropython)
