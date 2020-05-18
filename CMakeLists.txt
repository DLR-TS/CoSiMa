# CMakeList.txt: CMake-Projektdatei der obersten Ebene.
#
cmake_minimum_required (VERSION 3.8)

project ("CoSimulationManager")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "conan/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.14/conan.cmake"
                 "conan/conan.cmake")
endif()

include(conan/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt 
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)


# FMI4cpp is not distributed as conan package, but still can use conan for dependancy management.
conan_cmake_run(CONANFILE lib/FMI4cpp/conanfile.txt 
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)
add_subdirectory(lib/FMI4cpp/ EXCLUDE_FROM_ALL)
set(FMI4CPP_USING_CONAN ON)

# include protobuf, yaml_cpp, catch2, fakeit

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
conan_basic_setup()

# #protobuf
# add_subdirectory ("lib/protobuf/cmake")

#osi
# set (ENV{PATH} "${CMAKE_SOURCE_DIR}/lib/protobuf/build2/Debug")
# set (ENV{PROTOBUF} "${CMAKE_SOURCE_DIR}/cosima/lib/protobuf")
# set (ENV{PROTOBUF_SRC_ROOT_FOLDER} "%PROTOBUF%")
# set (ENV{CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/lib/protobuf/src")
# set (ENV{CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/lib/protobuf/build2/Debug")

# find_package(Protobuf REQUIRED)
# find_package(yaml_cpp)
# find_package(catch2)
# find_package(FakeIt)

# add_subdirectory ("lib/open-simulation-interface")

#fmi library
# add_subdirectory("lib/fmi-library")

# #test libraries
# add_subdirectory("lib/Catch2")
# add_subdirectory("lib/FakeIt")

#main project
# set up variables that are used below
add_subdirectory("src")
add_subdirectory("include")
add_subdirectory("test")
#create library to

add_library(CoSimulationManagerLib ${COSIMA_SOURCE} ${COSIMA_INCLUDES})
target_include_directories(CoSimulationManagerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(CoSimulationManagerLib ${CONAN_LIBS})#open_simulation_interface_static
# FMI4cpp is not managed by conan, thus it is added here
target_include_directories(CoSimulationManagerLib PUBLIC fmi4cpp)
target_link_libraries(CoSimulationManagerLib fmi4cpp)

find_package(Catch2 REQUIRED)
add_executable(test_CoSiMa ${TESTFILES})
target_include_directories(test_CoSiMa PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_CoSiMa CoSimulationManagerLib ${CONAN_LIBS} Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(test_CoSiMa)


add_executable (CoSimulationManager "${CMAKE_CURRENT_LIST_DIR}/src/CoSiMa.cpp" "${CMAKE_CURRENT_LIST_DIR}/include/CoSiMa.h")
target_link_libraries(CoSimulationManager CoSimulationManagerLib)