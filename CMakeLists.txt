cmake_minimum_required(VERSION 3.0.0)
project(gameboy VERSION 0.45.2)

include(CTest)
enable_testing()

find_package(SDL2 REQUIRED)
include_directories(gameboy ${SDL2_INCLUDE_DIRS})
file(GLOB SOURCES
    src/*.h
    src/*.cpp
    src/lib/*.h
    src/lib/*.cpp
)

add_executable(gameboy ${SOURCES})
target_link_libraries(gameboy ${SDL2_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
