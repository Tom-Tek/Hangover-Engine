cmake_minimum_required(VERSION 3.16)

project(Hangover VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------
# Qt Packages
# ------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ------------------------------------------
# Dummy Project
# ------------------------------------------
qt_standard_project_setup()

qt_add_executable(dummy
    main.cpp
)

target_link_libraries(dummy PRIVATE Qt6::Core)
target_link_libraries(dummy PRIVATE Qt6::Widgets)

set_target_properties(dummy PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

# ------------------------------------------
# Qt copy dlls when project built.
# ------------------------------------------
if(WIN32)
    # I'm going to die
    # I HATE QT DEPLOYMENT!!! IT'S SO GARBAGE
    add_custom_command(TARGET dummy POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:dummy> $<TARGET_FILE_DIR:dummy>
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:dummy>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt6::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:dummy>/platforms
        COMMAND_EXPAND_LISTS
    )
endif()