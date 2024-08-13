cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED ENV{SUMO_HOME})
    message(FATAL_ERROR "SUMO_HOME not found")
endif()

#Debug print
message(STATUS "SUMO_HOME = $ENV{SUMO_HOME}")

# Find SUMO library
if (UNIX)
	include_directories("$ENV{SUMO_HOME}/src")
endif(UNIX)
if(WIN32)
	include_directories("$ENV{SUMO_HOME}/include")
endif(WIN32)

link_directories("$ENV{SUMO_HOME}/bin")
