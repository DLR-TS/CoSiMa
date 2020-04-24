# CMakeList.txt: CMake-Projektdatei der obersten Ebene.
#
cmake_minimum_required (VERSION 3.8)

project ("CoSimulationManager")

#protobuf
add_subdirectory ("lib/protobuf/cmake")

#osi
set (ENV{PATH} "${CMAKE_SOURCE_DIR}/lib/protobuf/build2/Debug")
set (ENV{PROTOBUF} "${CMAKE_SOURCE_DIR}/cosima/lib/protobuf")
set (ENV{PROTOBUF_SRC_ROOT_FOLDER} "%PROTOBUF%")
set (ENV{CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/lib/protobuf/src")
set (ENV{CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/lib/protobuf/build2/Debug")
add_subdirectory ("lib/open-simulation-interface")

#fmi library
add_subdirectory("lib/fmi-library")

#yaml
add_subdirectory("lib/yaml-cpp")

#main project
add_subdirectory ("src")
