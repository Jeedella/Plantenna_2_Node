# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_node)

#Add compiler definitions and sources
if($ENV{SPMS_BT} STREQUAL "ble")
    add_definitions(-D__SPMS_BT=0)
    file(GLOB app_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ble/spms_ble.c)
    message(STATUS "Building SPMS node as ble")
elseif($ENV{SPMS_BT} STREQUAL "mesh_server")
    add_definitions(-D__SPMS_BT=1)
    file(GLOB app_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh/mesh_base_model.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh/mesh_sensor_setup_server.c)
    message(STATUS "Building SPMS node as mesh_server")
elseif($ENV{SPMS_BT} STREQUAL "mesh_client")
    add_definitions(-D__SPMS_BT=2)
    file(GLOB app_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh/mesh_base_model.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh/mesh_sensor_client.c)
    message(STATUS "Building SPMS node as mesh_client")
else()
    message(FATAL_ERROR "SPMS_BT environment variable not correctly defined\nValid options are ble, mesh_server and mesh_client")
endif()

# Add sources
target_include_directories(app PRIVATE GLOB ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/inc/ble ${CMAKE_CURRENT_SOURCE_DIR}/inc/mesh) 
target_sources(app PRIVATE ${app_sources})

# Build variables
# set(BOARD nrf52dk_nrf52832)
