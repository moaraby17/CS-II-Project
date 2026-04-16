cmake_minimum_required(VERSION 3.16)

project(DungeonRealms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/game.cpp
    src/level.cpp
    src/entity.cpp
    src/player.cpp
    src/enemy.cpp
    src/tile.cpp
)

set(HEADERS
    src/mainwindow.h
    src/game.h
    src/level.h
    src/entity.h
    src/player.h
    src/enemy.h
    src/tile.h
)

add_executable(DungeonRealms
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(DungeonRealms PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DungeonRealms)
endif()
