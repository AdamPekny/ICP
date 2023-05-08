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

add_executable(ICP_pacman src/main.cpp src/pacman.h src/pacman.cpp src/level.h src/level.cpp src/mapvector.h src/mapvector.cpp src/mapobjects.h src/mapobserverobject.h src/mapobjects.cpp src/widgets.cpp src/widgets.h src/leveloverlay.h src/leveloverlay.cpp src/styles.h src/widgets.cpp src/gamebar.cpp)
target_link_libraries(ICP_pacman
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )


