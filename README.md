# CoSimulationManager for SETLevel4to5

# Usage of the DLR research implementation:
The DLR research implementation consists at the moment out of 4 distinct programs. Start the following programs in this order:

1. Carla OSI
2. Carla OSI Service
3. OSMP Services for each FMU
4. CoSiMa with corresponding configuration file

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
To start CoSiMa a config file (yaml) needs to be provided. Since our goal is a broader cosimulation platform than the context of SetLevel needs, some configuration possibilities seem unnessesary.
The configuration contains information about all connected simulators.
Currently implemented are CARLA and OSMP and plain FMI.

## CARLA
Between CARLA and CoSiMa sits the CARLA-OSI-Client.\
The carla_host and carla_port address information are seen from the CARLA-OSI-Client.\
delta defines the simulation step size in seconds.

## OSMP
The OSMP simulator configuration contains the path to the FMU.\
A prefix is needed for internal use to mark OSI Messages. There should be no reason to change the value.\
The communication is established through the OSMPService. For each model an individual service is needed.

## FMI
The configuration for an FMU contains the path to the packed .fmu.\
Parameters can be set and outputs of the fmu are mapped with their interface_name to the base_name.\
Note: FMI-FMUs can not communicate directly with OSMP-FMUs.

### Input and Output
The interface_name defines the name of a OSI message, which the FMU expects.\
The base_name defines the name used by the base simulator (CARLA).\
This differentiation allows free adjustments between FMUs and the base simulator.\
FMUs can send information by any not yet defined base_name between each other.

## Parameters
| Parameter | Description |
| ------ | ------ |
| -d | Additional debug outputs. |
| -l | Create a log file, named cosima.log. No output in console. |
| -OSI | Additional OSI outputs to log or command line output. Be careful, since OSI messages can be very large. Potentially flooding the systems storage. |
| PATH.yaml | Path to Configuration file. |

## Used Libraries

cmake-conan 0.15\
gRPC\
protobuf\
yaml-cpp\
open-simulation-interface

# Contacts

bjoern.bahn@dlr.de
danny.behnecke@dlr.de
