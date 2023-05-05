cmake_minimum_required(VERSION 3.22)
project(ICP_pacman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ICP_pacman Sources/main.cpp Headers/pacman.h Sources/pacman.cpp Headers/level.h Sources/level.cpp Headers/mapvector.h Sources/mapvector.cpp Headers/mapobjects.h Headers/mapobserverobject.h Sources/mapobjects.cpp Headers/pathfinder.h Sources/pathfinder.cpp)
target_link_libraries(ICP_pacman
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

