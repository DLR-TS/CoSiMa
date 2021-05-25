# CoSimulationManager for SETLevel4to5

## Installation Guide

preparations:\
install cmake (version 3.12 or newer)\
install (conan.io)[conan.io] (version 1.29.x or newer )\
add conan.exe to PATH environment variable
<!--check out submodules (git submodule update --init --recursive) to get FMI4cpp or use GIT_SUBMODULE CMake option to do so automatically during build
	- FMI4cpp is not available as conan package (might change in the future?)
	- its dependencies are installed using conan, invoked from cmake when building CoSimulationManagerLib-->

# manual build
in root folder:
```sh
 mkdir build && cd build
 cmake .. -DCMAKE_BUILD_TYPE=Release # or 'cmake -D BUILD_SHARED_LIBS=false ..' on windows, see below
 cmake --build . --target CoSimulationManager 
 cmake --install . # not yet defined
```

 Because of broken dependencies using MSVC, build and use static libraries on windows by appending '-D BUILD_SHARED_LIBS=false' to all cmake generation calls

# building in Visual Studio 2017
just open the folder in Visual Studio and use the cmake integration

In Windows in den CMakeSettings.json "cmakeCommandArgs": "-D BUILD_SHARED_LIBS=false", einfügen.\
Do not build the libs shared.

use cmake for project generation

# CMake FetchContent Overrides
Some dependencies are retrieved using CMake's FetchContent Module. To override their source in your local repository, create a file named 'CMake_FetchContent_Overrides.cmake' and use the FetchContent_declare() to declare your desired replacement. The will be included by the root CmakeLists.txt file. CMake_FetchContent_Overrides.cmake is ignored and thus will not be added to the global repository. 

LibCarla_client, its dependencies and gRPC currently are included using FetchContent.

# Configure CoSiMa
To start CoSiMa a YAML File needs to be provided. Since our goal is a broader cosimulation platform than the context of SetLevel needs, some configuration possibilities seem unnessesary.
The configuration contains information about all connected simulators.
Currently implemented are CARLA and OSMP.

## CARLA
Between CARLA and CoSiMa sits the CARLA-OSI-Client.\
The carla_host and carla_port address information are seen from the CARLA-OSI-Client.\
delta defines the step size in seconds.

## OSMP
The OSMP simulator configuration contains the path to the FMU.\
A prefix is needed for internal use to mark OSI Messages. There should be no reason to change the value.

### Input and Output
The interface_name defines the name of a OSI message, which the FMU expects.\
The base_name defines the name used by the base simulator (CARLA).\
This differentiation allows free adjustments between FMUs and the base simulator.\
FMUs can send information by any not yet defined base_name between each other.

## Used Libraries

cmake-conan 0.15\
LibCarla_client + dependencies (Carla 0.9.9.4)\
gRPC\
protobuf\
yaml-cpp\
open-simulation-interface
