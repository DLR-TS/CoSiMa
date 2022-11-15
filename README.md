# OSTAR CoSimulationManager

## Usage of the OSTAR implementation
See [Quickstart](https://github.com/DLR-TS/OSTAR-Quickstart).

### Docker build

```sh
 docker build -t ostar:cosima .
```

The docker container contains no configuration file. Import it by docker volume and add the according path as a runtime parameter. See example at [Quickstart](https://github.com/DLR-TS/OSTAR-Quickstart).

## Manual build

preparations:\
install [cmake](https://cmake.org/) (version 3.12 or newer)\
install [conan.io](https://conan.io/) (version 1.29.x or newer )\
add conan.exe to PATH environment variable

### Linux

in root folder:
```sh
 mkdir build && cd build
 cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=false ..
 cmake --build . --target CoSimulationManager
```

### Windows with MSVC 2017
Open the folder in Visual Studio and use the cmake integration.


### CMake FetchContent Overrides
Some dependencies are retrieved using CMake's FetchContent Module. To override their source in your local repository, create a file named 'CMake_FetchContent_Overrides.cmake' and use the FetchContent_declare() to declare your desired replacement. They will be included by the root CMakeLists.txt file. CMake_FetchContent_Overrides.cmake is ignored and thus will not be added to the global repository. 

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
| -v | Verbose outputs. |
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

This software was originally developed as part of SetLevel.
