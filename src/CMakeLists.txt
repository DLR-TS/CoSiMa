#version
cmake_minimum_required (VERSION 3.8)

add_subdirectory("mapper")
add_subdirectory("simulation_interfaces")
add_subdirectory(configreader)

#source
set(COSIMA_SOURCE ${COSIMA_SOURCE}
"${CMAKE_CURRENT_LIST_DIR}/SimulationInterfaceFactory.cpp" 

PARENT_SCOPE
)

