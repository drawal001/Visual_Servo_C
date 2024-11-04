cmake_minimum_required(VERSION 3.10)
project(GalaxySDK VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(GalaxySDK SHARED IMPORTED)

# set_target_properties(GalaxySDK PROPERTIES
# IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/APIDll/Win64/DxImageProc.dll"
# )
set_target_properties(GalaxySDK PROPERTIES
    IMPORTED_IMPLIB
    "${PROJECT_SOURCE_DIR}/Development/VC SDK/lib/x64/GxIAPI.lib"
)

get_target_property(
    xxxx GalaxySDK IMPORTED_IMPLIB
)

message(${xxxx})

add_library(GalaxyCamera GalaxyCamera.cpp)
target_link_libraries(GalaxyCamera GalaxySDK)

include_directories("${PROJECT_SOURCE_DIR}/Development/VC SDK/inc")

list(APPEND CMAKE_PREFIX_PATH E:\\WYL_workspace\\OpenCV_4.10\\x64\\mingw\\bin\\)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

if(OpenCV_FOUND)
    target_link_libraries(GalaxyCamera ${OpenCV_LIBS})
else()
    message("ddd")
endif()

add_executable(Main main.cpp)
target_link_libraries(Main GalaxyCamera)
target_include_directories(Main PUBLIC "${PROJECT_SOURCE_DIR}/Development/VC SDK/inc")