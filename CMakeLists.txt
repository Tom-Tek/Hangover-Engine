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
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

# ------------------------------------------
# Helper: deploy Qt next to an exe after build
# windeployqt copies the Qt DLLs and every plugin the exe needs (platforms, styles, imageformats, ...)
# ------------------------------------------
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_BINS}" REQUIRED)

function(deploy_qt target)
    if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
            # Non-Qt DLLs this exe links, e.g. core
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                $<IF:$<CONFIG:Debug>,--debug,--release>
                --no-translations
                --no-compiler-runtime
                --no-system-d3d-compiler
                --no-opengl-sw
                --no-system-dxc-compiler
                --skip-plugin-types tls,networkinformation,generic
                --verbose 0
                $<TARGET_FILE:${target}>
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

# enact only ships a Release (/MD) build, see liquor/CMakeLists.txt
add_library(enact STATIC IMPORTED GLOBAL)
set_target_properties(enact PROPERTIES
    IMPORTED_LOCATION ${THIRDPARTY_DIR}/enact/lib/enact.lib
    INTERFACE_INCLUDE_DIRECTORIES ${THIRDPARTY_DIR}/enact/include
)

# ------------------------------------------
# Projects
# ------------------------------------------
add_subdirectory(core)
add_subdirectory(plastered)
add_subdirectory(liquor)

# ------------------------------------------
# resources: copied next to the executables so tools never depend on the source tree
# Runs on every build; edits to resources/ need a build to show up.
# ------------------------------------------
add_custom_target(resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:liquor>/resources
    COMMENT "Copying resources next to the executables"
)