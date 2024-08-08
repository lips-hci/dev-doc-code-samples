cmake_minimum_required(VERSION 3.3)

project(camera-parameter CXX)

include_directories($ENV{OPENNI2_INCLUDE})
include_directories($ENV{OPENNI2_INCLUDE64})
link_directories($ENV{OPENNI2_REDIST})
link_directories($ENV{OPENNI2_LIB64})

add_executable(${PROJECT_NAME} camera-parameter.cpp)

target_link_libraries(${PROJECT_NAME} OpenNI2)

if(MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OPENNI2_REDIST64}/OpenNI2.dll" ${CMAKE_BINARY_DIR}/Release/
        COMMAND ${CMAKE_COMMAND} -E copy_directory "$ENV{OPENNI2_REDIST64}/OpenNI2/" ${CMAKE_BINARY_DIR}/Release/OpenNI2/
    )
endif()