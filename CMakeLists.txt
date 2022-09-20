cmake_minimum_required(VERSION 3.0.0)

project(gameboy VERSION 0.46.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

include_directories(gameboy)
file(GLOB SOURCES
    src/*.h
    src/*.cpp
    src/lib/*.h
    src/lib/*.cpp
)

qt_add_executable(gameboy ${SOURCES}
    MANUAL_FINALIZATION
)
target_link_libraries(gameboy PRIVATE Qt::Widgets)

qt_finalize_executable(gameboy)
