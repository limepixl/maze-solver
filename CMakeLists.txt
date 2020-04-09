cmake_minimum_required(VERSION 3.13)

project(maze-solver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Wpedantic -O2")

file(GLOB SRC "*.cpp" "*.h")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} sfml-graphics)
