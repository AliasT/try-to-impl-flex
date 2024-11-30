cmake_minimum_required(VERSION 3.10)



project(HelloWorld)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

find_package(fmt CONFIG REQUIRED)

find_package(SDL2 CONFIG REQUIRED)

add_executable(app main.cc)



target_link_libraries(app PRIVATE fmt::fmt)
target_link_libraries(app PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static)