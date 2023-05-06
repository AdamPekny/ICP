cmake_minimum_required(VERSION 3.22)
project(cmake-build-debug/ICP_pacman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ICP_pacman Sources/main.cpp Headers/pacman.h Sources/pacman.cpp Headers/level.h Sources/level.cpp Headers/mapvector.h Sources/mapvector.cpp Headers/mapobjects.h Headers/mapobserverobject.h Sources/mapobjects.cpp Sources/widgets.cpp Headers/widgets.h Headers/leveloverlay.h Sources/leveloverlay.cpp Headers/styles.h)
target_link_libraries(ICP_pacman
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

