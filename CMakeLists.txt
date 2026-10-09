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
# Submodules
# ------------------------------------------
#add_subdirectory(submodules/EASTL)

# ------------------------------------------
# Projects
# ------------------------------------------
add_subdirectory(plastered)