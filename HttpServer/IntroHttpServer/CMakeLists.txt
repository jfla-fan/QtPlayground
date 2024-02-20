cmake_minimum_required(VERSION 3.14)

project(IntroHttpServer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core HttpServer Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core HttpServer Concurrent)

#qt_add_resources(
#    ${PROJECT_NAME}
#        "res"
#    PREFIX
#        "/assets"
#    FILES
#        ${resource_files}
#)

add_executable(IntroHttpServer
    main.cpp
    resources.qrc
    utils.h
    utils.cpp
    db/fileStorage.h
    db/private/systemFileStorage.h
    db/private/systemFileStorage.cpp
    db/private/scopedFileStorage.h
    db/private/scopedFileStorage.cpp
    serverApi.h serverApi.cpp
    res.hpp
)

target_link_libraries(IntroHttpServer Qt${QT_VERSION_MAJOR}::Core
                                      Qt${QT_VERSION_MAJOR}::HttpServer
                                      Qt${QT_VERSION_MAJOR}::Concurrent)

include(GNUInstallDirs)
install(TARGETS IntroHttpServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
