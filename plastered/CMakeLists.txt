# ------------------------------------------
# Copyright (c) 2026 Liam Rousselle
# All rights reserved
# ------------------------------------------

# ------------------------------------------
# Project files
# ------------------------------------------
file(GLOB_RECURSE PLASTERED_SOURCES CONFIGURE_DEPENDS source/*.cpp)
file(GLOB_RECURSE PLASTERED_HEADERS CONFIGURE_DEPENDS source/*.h)

qt_add_executable(plastered ${PLASTERED_SOURCES} ${PLASTERED_HEADERS})

target_link_libraries(plastered PRIVATE Qt6::Core)
target_link_libraries(plastered PRIVATE Qt6::Widgets)

copy_qt_dlls(plastered)