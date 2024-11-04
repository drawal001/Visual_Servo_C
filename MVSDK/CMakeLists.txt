cmake_minimum_required(VERSION 3.10)
project(MVSDK VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(MVConfig MVConfig.cpp)
add_library(MVSDK SHARED IMPORTED)

set_target_properties(MVSDK PROPERTIES
    IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/x64/MVCAMSDK_X64.lib"
)
set_target_properties(MVSDK PROPERTIES
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/x64/MVCAMSDK_X64.dll"
)

target_link_libraries(MVConfig MVSDK)

add_executable(Main main.cpp)

target_link_libraries(Main MVConfig)
target_include_directories(Main PUBLIC "${PROJECT_SOURCE_DIR}/Demo/VC++/Include")