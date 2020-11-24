# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_node)

# Add sources
file(GLOB app_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
target_include_directories(app PRIVATE GLOB ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_sources(app PRIVATE ${app_sources})

# Build variables
# set(BOARD nrf52dk_nrf52832)