cmake_minimum_required(VERSION 3.12)
include(ExternalProject)

# Find SUMO library
if(UNIX)
	find_path(SUMO_INCLUDE_DIR NAMES "sumo.h" PATHS "/usr/include/sumo" "/usr/local/include/sumo")
	find_library(SUMO_LIBRARY NAMES "sumo" PATHS "/usr/lib" "/usr/local/lib")
endif(UNIX)
if(WIN32)
	find_path(SUMO_INCLUDE_DIR NAMES "libsumo/libsumo.h" PATHS "C:/Program Files (x86)/Eclipse/Sumo/include")
	find_library(SUMO_LIBRARY NAMES "libsumocpp.lib" PATHS "C:/Program Files (x86)/Eclipse/Sumo/bin")
endif()

if(NOT SUMO_INCLUDE_DIR OR NOT SUMO_LIBRARY)
    message(FATAL_ERROR "SUMO library not found")
endif()

# Include SUMO headers
include_directories(${SUMO_INCLUDE_DIR})
