add_subdirectory(mapper)
add_subdirectory(simulation_interfaces)
add_subdirectory(reader)
add_subdirectory(base_interfaces)

set(COSIMA_INCLUDES ${MAPPER_INCLUDES} ${SIMULATION_INTERFACE_INCLUDES} ${CONFIGREADER_INCLUDES} ${BASE_INTERFACE_INCLUDES}
${CMAKE_CURRENT_LIST_DIR}/ScenarioRunnerAdapter.h
${CMAKE_CURRENT_LIST_DIR}/SubProcessController.h
${CMAKE_CURRENT_LIST_DIR}/CoSiMaUtility.h
${CMAKE_CURRENT_LIST_DIR}/CoSiMa.h
PARENT_SCOPE
)