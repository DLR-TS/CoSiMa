set (TESTFILES 
${CMAKE_CURRENT_LIST_DIR}/tests-main.cpp

${CMAKE_CURRENT_LIST_DIR}/tests-yaml_reader.cpp
${CMAKE_CURRENT_LIST_DIR}/test-isimulationdata.cpp
${CMAKE_CURRENT_LIST_DIR}/tests-mapper.cpp
# simulation interfaces
${CMAKE_CURRENT_LIST_DIR}/tests-FMIInterface.cpp
# base interfaces
$<$<BOOL:${WITH_CARLA}>:${CMAKE_CURRENT_LIST_DIR}/tests-CARLAInterface.cpp>
${CMAKE_CURRENT_LIST_DIR}/tests-simulationinterfacefactory.cpp
${CMAKE_CURRENT_LIST_DIR}/tests-OSMPInterface.cpp
${CMAKE_CURRENT_LIST_DIR}/tests-OSIBridge.cpp

${CMAKE_CURRENT_LIST_DIR}/MockBaseSimulator.cpp
${CMAKE_CURRENT_LIST_DIR}/MockInterfaceSimulator.cpp
${CMAKE_CURRENT_LIST_DIR}/MockMapper.cpp
$<$<BOOL:${WITH_CARLA}>:${CMAKE_CURRENT_LIST_DIR}/MockGRPCBaseSimulatorServer.cpp>

PARENT_SCOPE)