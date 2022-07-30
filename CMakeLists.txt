cmake_minimum_required(VERSION 3.0.0)
project(gameboy VERSION 0.45.1)

include(CTest)
enable_testing()

find_package(SDL2 REQUIRED)
include_directories(gameboy ${SDL2_INCLUDE_DIRS})
file(GLOB SOURCES
    src/*.h
    src/*.cpp
    src/clock/*.h
    src/clock/*.cpp
    src/cpu/*.h
    src/cpu/*.cpp
    src/display/*.h
    src/display/*.cpp
    src/emudebug/*.h
    src/emudebug/*.cpp
    src/emulator/*.h
    src/emulator/*.cpp
    src/interrupt/*.h
    src/interrupt/*.cpp
    src/memory/*.h
    src/memory/*.cpp
    src/render/*.h
    src/render/*.cpp
    src/render/spriteattrib/*.h
    src/render/spriteattrib/*.cpp
)

add_executable(gameboy ${SOURCES})
target_link_libraries(gameboy ${SDL2_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
