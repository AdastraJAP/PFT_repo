cmake_minimum_required(VERSION 3.20)
project(PFT LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC "src/*.cpp")
add_executable(pft ${SRC})
target_include_directories(pft PRIVATE ${CMAKE_SOURCE_DIR}/include)