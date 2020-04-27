#version
cmake_minimum_required (VERSION 3.8)

add_subdirectory("Mapper")
add_subdirectory("SimulationInterfaces")

#source
set(COSIMA_SOURCE ${COSIMA_SOURCE}
"CoSiMa.cpp" "CoSiMa.h"
"SimulationInterfaceFactory.cpp" "SimulationInterfaceFactory.h"
)

add_executable (CoSimulationManager ${COSIMA_SOURCE})

add_library(CoSimulationManagerLib ${COSIMA_SOURCE})

target_link_libraries(CoSimulationManager open_simulation_interface_static)

target_link_libraries(CoSimulationManager yaml-cpp)