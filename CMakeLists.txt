cmake_minimum_required(VERSION 3.20)
project(structurepad)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find FTXUI package
find_package(ftxui CONFIG REQUIRED)

# Add executable
add_executable(structurepad 
    src/main.cpp
)

# Link FTXUI library
target_link_libraries(structurepad PRIVATE ftxui::screen ftxui::dom ftxui::component)

# Include directories
target_include_directories(structurepad PRIVATE ${CMAKE_SOURCE_DIR}/include)
