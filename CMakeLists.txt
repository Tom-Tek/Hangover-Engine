# ------------------------------------------
# Copyright (c) 2026 Liam Rousselle
# All rights reserved
# ------------------------------------------
cmake_minimum_required(VERSION 3.21)

project(Hangover VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------
# Qt Packages
# ------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

# ------------------------------------------
# Helper: copy Qt DLLs next to an exe after build
# ------------------------------------------
function(copy_qt_dlls target)
    if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
            COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target}>/platforms
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Qt6::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${target}>/platforms
            COMMAND_EXPAND_LISTS
        )
    endif()
endfunction()

# ------------------------------------------
# Thirdparty (prebuilt)
# ------------------------------------------
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

# EASTL is prebuilt once into thirdparty/eastl, Debug and Release flavours
add_library(EASTL STATIC IMPORTED GLOBAL)
set_target_properties(EASTL PROPERTIES
    IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
    IMPORTED_LOCATION_DEBUG ${THIRDPARTY_DIR}/eastl/lib/Debug/EASTL.lib
    IMPORTED_LOCATION_RELEASE ${THIRDPARTY_DIR}/eastl/lib/Release/EASTL.lib
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    INTERFACE_INCLUDE_DIRECTORIES ${THIRDPARTY_DIR}/eastl/include
    INTERFACE_SOURCES ${THIRDPARTY_DIR}/eastl/EASTL.natvis
)

# ------------------------------------------
# Projects
# ------------------------------------------
add_subdirectory(core)
add_subdirectory(plastered)