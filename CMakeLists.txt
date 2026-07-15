cmake_minimum_required(VERSION 3.10)
project(Millionaire)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

file(GLOB SOURCES "*.cpp")

add_executable(Millionaire ${SOURCES})

target_link_libraries(Millionaire sfml-graphics sfml-window sfml-system sfml-audio )