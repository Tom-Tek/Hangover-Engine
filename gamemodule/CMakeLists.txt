# ------------------------------------------
# Copyright (c) 2026 Liam Rousselle
# All rights reserved
# ------------------------------------------

# ------------------------------------------
# gamemodule: engine-owned code linked into every game module (hangover-game.lib).
# Shipped prebuilt in bin/ so game projects never contain (or edit) its source.
# ------------------------------------------
file(GLOB_RECURSE GAMEMODULE_SOURCES CONFIGURE_DEPENDS source/*.cpp)
file(GLOB_RECURSE GAMEMODULE_HEADERS CONFIGURE_DEPENDS source/*.h)

add_library(gamemodule STATIC ${GAMEMODULE_SOURCES} ${GAMEMODULE_HEADERS})

set_target_properties(gamemodule PROPERTIES OUTPUT_NAME hangover-game)
