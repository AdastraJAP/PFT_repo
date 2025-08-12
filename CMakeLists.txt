cmake_minimum_required(VERSION 3.20)
project(PFT LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Find SQLite3
find_package(SQLite3 REQUIRED)

# Source files
file(GLOB SRC "src/*.cpp")

# Create executable
add_executable(pft ${SRC})

# Include directories
target_include_directories(pft PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link SQLite3
target_link_libraries(pft PRIVATE SQLite::SQLite3)

# Set output directory
set_target_properties(pft PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)