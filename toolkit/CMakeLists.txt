# ------------------------------------------
# Copyright (c) 2026 Liam Rousselle
# All rights reserved
# ------------------------------------------

# ------------------------------------------
# toolkit: code the development tools (liquor, plastered) need and games never do,
# e.g. engine registration, project creation, packaging, asset importing.
# Source-only (INTERFACE): every tool compiles it with its own settings, so liquor's
# release-CRT workaround never clashes with a prebuilt library.
# Never linked into packaged games.
# ------------------------------------------
file(GLOB_RECURSE TOOLKIT_SOURCES CONFIGURE_DEPENDS source/*.cpp)
file(GLOB_RECURSE TOOLKIT_HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(toolkit INTERFACE)
target_sources(toolkit INTERFACE ${TOOLKIT_SOURCES} ${TOOLKIT_HEADERS})

target_include_directories(toolkit INTERFACE include)

target_compile_definitions(toolkit INTERFACE HANGOVER_BIN_DIR="${HANGOVER_BIN_DIR}")

if(WIN32)
    target_link_libraries(toolkit INTERFACE advapi32 user32 shell32 ole32)
endif()
