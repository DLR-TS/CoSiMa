add_subdirectory(mapper)
add_subdirectory(simulation_interfaces)

set(COSIMA_INCLUDES ${MAPPER_INCLUDES} ${SIMULATION_INTERFACE_INCLUDES}
"SimulationInterfaceFactory.h"
PARENT_SCOPE
)