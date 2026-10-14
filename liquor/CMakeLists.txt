# ------------------------------------------
# Copyright (c) 2026 Liam Rousselle
# All rights reserved
# ------------------------------------------

# ------------------------------------------
# Project files
# ------------------------------------------
file(GLOB_RECURSE LIQUOR_SOURCES CONFIGURE_DEPENDS source/*.cpp)
file(GLOB_RECURSE LIQUOR_HEADERS CONFIGURE_DEPENDS source/*.h)

add_executable(liquor ${LIQUOR_SOURCES} ${LIQUOR_HEADERS})

target_include_directories(liquor PUBLIC source)

# Static core: the core DLL is built with the debug CRT in Debug, which would mix CRTs, see below
target_link_libraries(liquor PRIVATE core-static)
target_link_libraries(liquor PRIVATE enact)
target_link_libraries(liquor PRIVATE EASTL)

# enact.lib is only shipped as a Release (/MD) build, so liquor always uses the
# release CRT and release EASTL to avoid _ITERATOR_DEBUG_LEVEL / RuntimeLibrary mismatches.
# Remove this once a Debug enact.lib is available.
set_target_properties(liquor PROPERTIES
    MSVC_RUNTIME_LIBRARY MultiThreadedDLL
    MAP_IMPORTED_CONFIG_DEBUG Release
)
