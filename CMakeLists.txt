cmake_minimum_required(VERSION 3.22)
project(ICP_pacman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ICP_pacman Sources/main.cpp Headers/pacman.h Sources/pacman.cpp Headers/config.h Headers/mapcell.h Sources/mapcell.cpp Headers/map.h Sources/map.cpp Headers/gamestate.h Sources/gamestate.cpp Sources/ghost.cpp Headers/ghost.h)
target_link_libraries(ICP_pacman
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

