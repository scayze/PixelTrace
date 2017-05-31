# Specify the minimum version for CMake

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Project's name
PROJECT(ColorTrace C CXX)

# Set the output folder where your program will be created
SET(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# enable C++11 standard
SET(CMAKE_CXX_STANDARD 11)  

# The following folder will be included
SET(SOURCES main.cpp utils.cpp)

ADD_EXECUTABLE(ColorTrace ${SOURCES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Detect and add SFML
find_package(POTRACE)
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
  include_directories(
    ${SFML_INCLUDE_DIR}
    ${POTRACE_INCLUDE_DIR}
    )
  target_link_libraries(ColorTrace
  ${SFML_LIBRARIES}
  ${POTRACE_LIBRARIES}
  )
endif()

