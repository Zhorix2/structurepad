cmake_minimum_required(VERSION 3.20)
project(structurepad)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find PDCurses package
find_package(pdcurses CONFIG REQUIRED)

# Add executable
add_executable(structurepad 
    src/main.cpp
)

# Link PDCurses library
target_link_libraries(structurepad PRIVATE pdcurses::pdcurses)

# Include directories
target_include_directories(structurepad PRIVATE ${CMAKE_SOURCE_DIR}/include)
