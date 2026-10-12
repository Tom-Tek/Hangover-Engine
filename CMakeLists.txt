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

# The static QWindowKit build links Qt's private modules, which Qt 6.10+ only provides when asked for.
# Private modules tie the editor to this exact Qt build: rebuild thirdparty/qwindowkit when upgrading Qt.
set(QT_NO_PRIVATE_MODULE_WARNING ON)
find_package(Qt6 REQUIRED COMPONENTS CorePrivate GuiPrivate WidgetsPrivate)
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

# QWindowKit (editor only): custom title bar with native snapping, shadows and resizing.
# Prebuilt static, Debug + Release, and found through its own CMake package.
find_package(QWindowKit REQUIRED PATHS ${THIRDPARTY_DIR}/qwindowkit NO_DEFAULT_PATH)

# ------------------------------------------
# bin: everything game code may use, inside this build's output
#   bin/include   headers
#   bin/          .dll / .lib / .pdb
# HANGOVER_BIN points at the bin of the most recently launched build (registered by liquor and
# the editor on launch), so game code always matches the running editor's configuration.
# A plain variable on purpose: it overrides the repo-root path older build folders cached.
# ------------------------------------------
set(HANGOVER_BIN_DIR ${CMAKE_BINARY_DIR}/bin)

# Multi-config generators (e.g. the Visual Studio generator) share one build folder
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTI_CONFIG)
    set(HANGOVER_BIN_DIR ${HANGOVER_BIN_DIR}/$<CONFIG>)
endif()

# ------------------------------------------
# Projects
# ------------------------------------------
add_subdirectory(toolkit)
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

# ------------------------------------------
# bin export: runs on every build so header-only edits are picked up too.
# Add new game-facing libraries (e.g. Yeast) here.
#   sdk/        engine files exported as-is (premake/hangover.lua, source/GameModule.cpp)
#   premake5    the generator projects run through $HANGOVER_BIN/premake
# ------------------------------------------
if(WIN32)
    set(PREMAKE_EXECUTABLE ${THIRDPARTY_DIR}/premake5/premake5.exe)
else()
    set(PREMAKE_EXECUTABLE ${THIRDPARTY_DIR}/premake5/premake5)
endif()

# Tells hangover.lua which configuration this bin holds, so projects only offer that one
file(GENERATE
    OUTPUT ${HANGOVER_BIN_DIR}/premake/engine.lua
    CONTENT "-- Generated by the engine build, do not edit\nhangover.config = \"$<CONFIG>\"\n"
)

add_custom_target(bin ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${HANGOVER_BIN_DIR}/include ${HANGOVER_BIN_DIR}/premake

    # Engine-owned SDK files and the project generator
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/sdk ${HANGOVER_BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PREMAKE_EXECUTABLE} ${HANGOVER_BIN_DIR}/premake

    # Headers
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/core/include ${HANGOVER_BIN_DIR}/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${THIRDPARTY_DIR}/eastl/include ${HANGOVER_BIN_DIR}/include

    # Libraries (pdb only exists when the configuration has debug info)
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:core>
        $<TARGET_LINKER_FILE:core>
        $<$<CONFIG:Debug,RelWithDebInfo>:$<TARGET_PDB_FILE:core>>
        $<TARGET_FILE:EASTL>
        ${HANGOVER_BIN_DIR}

    COMMAND_EXPAND_LISTS
    COMMENT "Exporting game SDK to bin/"
)
add_dependencies(bin core)