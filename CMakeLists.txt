cmake_minimum_required(VERSION 3.0.0)

project(gameboy VERSION 0.46.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(SOURCES
    src/displayconst.h
    src/Clock.h
    src/Clock.cpp
    src/cpu.h
    src/cpu.cpp
    src/display.h
    src/display.cpp
    src/DebugWindow.h
    src/DebugWindow.cpp
    src/GameBoyView.h
    src/GameBoyView.cpp
    src/interrupt.h
    src/interrupt.cpp
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/Memory.h
    src/Memory.cpp
    src/render.h
    src/render.cpp
    src/spriteattrib.h
    src/spriteattrib.cpp
    src/System.h
    src/System.cpp
    src/SystemDebug.h
    src/SystemDebug.cpp
    src/LibCB/Error.h
    src/LibCB/Types.h
)

qt_add_executable(gameboy ${SOURCES}
    MANUAL_FINALIZATION
)
target_link_libraries(gameboy PRIVATE Qt::Widgets)

qt_finalize_executable(gameboy)
