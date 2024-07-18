cmake_minimum_required(VERSION 3.3)

project(range-filter CXX)

include_directories($ENV{OPENNI2_INCLUDE})
include_directories($ENV{OPENNI2_INCLUDE64})
link_directories($ENV{OPENNI2_REDIST})
link_directories($ENV{OPENNI2_LIB64})

add_executable(${PROJECT_NAME} range-filter.cpp)

if(MSVC)
    include(../opencv_path.cmake)
endif()
find_package(OpenCV REQUIRED)

target_link_libraries(${PROJECT_NAME} OpenNI2)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

if(MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OPENNI2_REDIST64}/OpenNI2.dll" ${CMAKE_BINARY_DIR}/Release/
        COMMAND ${CMAKE_COMMAND} -E copy_directory "$ENV{OPENNI2_REDIST64}/OpenNI2/" ${CMAKE_BINARY_DIR}/Release/OpenNI2/
    )
endif()