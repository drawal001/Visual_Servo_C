cmake_minimum_required(VERSION 3.10)
project(Visual_Servo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


list(APPEND CMAKE_PREFIX_PATH E:\\WYL_workspace\\OpenCV_4.10\\x64\\mingw\\bin\\)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Main main.cpp)
add_executable(GetPicture ${PROJECT_SOURCE_DIR}/Calibration/GetPicture.cpp)

# add_subdirectory("${CMAKE_SOURCE_DIR}/GalaxySDK")
# target_link_libraries(Main GalaxyCamera)

if(OpenCV_FOUND)
target_link_libraries(Main ${OpenCV_LIBS})
target_link_libraries(GetPicture ${OpenCV_LIBS})
else()
message(SEND_ERROR "link_error")
endif()