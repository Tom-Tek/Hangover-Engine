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

target_include_directories(plastered PUBLIC source)

target_link_libraries(plastered PRIVATE toolkit)
target_link_libraries(plastered PRIVATE core)
target_link_libraries(plastered PRIVATE Qt6::Core)
target_link_libraries(plastered PRIVATE Qt6::Gui)
target_link_libraries(plastered PRIVATE Qt6::Widgets)
target_link_libraries(plastered PRIVATE QWindowKit::Widgets)

deploy_qt(plastered)
