cmake_minimum_required(VERSION 3.0.0)

project(card)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_executable(card 
    src/main.cpp
    src/utils.cpp)

# Add lib: rapidcsv
find_path(RAPIDCSV_INCLUDE_DIRS "rapidcsv.h")
target_include_directories(card PRIVATE ${RAPIDCSV_INCLUDE_DIRS})

# Add lib: tabulate
find_package(tabulate CONFIG REQUIRED)
target_link_libraries(card PRIVATE tabulate::tabulate)