# ------------------------------------------
# Copyright (c) 2026 Liam Rousselle
# All rights reserved
# ------------------------------------------

# ------------------------------------------
# Code shared by the development tools (liquor, plastered).
# Source-only (INTERFACE): every tool compiles it with its own settings, so liquor's
# release-CRT workaround never clashes with a prebuilt library.
# Never linked into packaged games.
# ------------------------------------------
file(GLOB_RECURSE SHARED_SOURCES CONFIGURE_DEPENDS source/*.cpp)
file(GLOB_RECURSE SHARED_HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(shared INTERFACE)
target_sources(shared INTERFACE ${SHARED_SOURCES} ${SHARED_HEADERS})

target_include_directories(shared INTERFACE include)

target_compile_definitions(shared INTERFACE HANGOVER_BIN_DIR="${HANGOVER_BIN_DIR}")

if(WIN32)
    target_link_libraries(shared INTERFACE advapi32 user32 shell32 ole32)
endif()
