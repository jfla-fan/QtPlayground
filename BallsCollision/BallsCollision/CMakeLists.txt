cmake_minimum_required(VERSION 3.16)
project(BallsCollision LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    mainWindow.ui
    mainWindow.h
    mainWindow.cpp
    ball.h
    ball.cpp
    collisionStrategy.cpp
)

qt_add_executable(BallsCollision ${PROJECT_SOURCES}
    config.hpp
    arrow.h
    arrow.cpp
    collisionStrategy.h )

set_target_properties(BallsCollision
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(BallsCollision
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
