# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.20.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_node)
zephyr_compile_definitions(PROJECT_NAME=${PROJECT_NAME} VENDOR_EVENT_DONE_MAX=5)
#Add compiler definitions and sources
message(STATUS "SPMS is Node") 
add_definitions(-D__SPMS_BT=1)
file(GLOB app_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh/mesh_base_model.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh/mesh_sensor_setup_server.c)
message(STATUS "Building SPMS as Node")
# Add sources
target_include_directories(app PRIVATE GLOB ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/inc/ble ${CMAKE_CURRENT_SOURCE_DIR}/inc/mesh) 
target_sources(app PRIVATE ${app_sources})

# Build variables
# set(BOARD nrf52dk_nrf52832)
