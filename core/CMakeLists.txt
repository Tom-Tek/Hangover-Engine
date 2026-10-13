# ------------------------------------------
# Copyright (c) 2026 Liam Rousselle
# All rights reserved
# ------------------------------------------

# ------------------------------------------
# Project files
# ------------------------------------------
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS source/*.cpp)
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(core SHARED ${CORE_SOURCES} ${CORE_HEADERS})

# Built as hangover-core.dll: it ships next to every game, so "core.dll" alone could clash with other libraries
set_target_properties(core PROPERTIES OUTPUT_NAME hangover-core)

target_include_directories(core PUBLIC include)

target_link_libraries(core PUBLIC EASTL)

# ------------------------------------------
# hangover-game.lib: engine-owned code linked into every game module (e.g. EASTL's
# allocation hooks). Kept out of source/ so it is not compiled into hangover-core.dll;
# shipped prebuilt in bin/ so game projects never contain (or edit) its source.
# ------------------------------------------
file(GLOB_RECURSE GAMEMODULE_SOURCES CONFIGURE_DEPENDS game/*.cpp)

add_library(gamemodule STATIC ${GAMEMODULE_SOURCES})

set_target_properties(gamemodule PROPERTIES OUTPUT_NAME hangover-game)