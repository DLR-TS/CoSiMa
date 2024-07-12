cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED ENV{SUMO_HOME})
    message(FATAL_ERROR "SUMO_HOME not found")
endif()

#Debug print
message(STATUS "SUMO_HOME = $ENV{SUMO_HOME}")

# Find SUMO library
include_directories("$ENV{SUMO_HOME}/src")
link_directories("$ENV{SUMO_HOME}/bin")