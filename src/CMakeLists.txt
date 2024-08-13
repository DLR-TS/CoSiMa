#version
cmake_minimum_required (VERSION 3.8)

add_subdirectory(mapper)
add_subdirectory(simulation_interfaces)
add_subdirectory(reader)
add_subdirectory(base_interfaces)

#source
set(COSIMA_SOURCE ${COSIMA_SOURCE}
${CMAKE_CURRENT_LIST_DIR}/SubProcessController.cpp
${CMAKE_CURRENT_LIST_DIR}/ScenarioRunnerAdapter.cpp
${CMAKE_CURRENT_LIST_DIR}/CoSiMaUtility.cpp
${CMAKE_CURRENT_LIST_DIR}/CoSiMa.cpp
PARENT_SCOPE
)

