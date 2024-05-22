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
install [conan.io](https://conan.io/) (version 1.x, NOT 2.x!)\
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
To start CoSiMa a config file (yaml) needs to be provided.\
The configuration contains information about all connected simulators.\
For a detailed description see the [documentation here](https://github.com/DLR-TS/CoSiMa/blob/master/Configuration.md).
Currently implemented are connectors to CARLA and to OSMP, as well as a Dummy to run without CARLA.

## CARLA
Between CARLA and CoSiMa sits the CARLA-OSI-Service.\
The carla_host and carla_port address information are seen from the CARLA-OSI-Client.\
delta defines the simulation step size in seconds.\
It is possible to run a simulation with a dummy interface without the CARLA-OSI-Service.

## OSMP
The OSMP simulator configuration contains the path to the FMU.\
A prefix is needed for internal use to mark OSI Messages.\
There should be no reason to change the value.\
The communication is established through the OSMP-Service.\
For each OSMP model an individual service is needed.\
The OSMP-Service can also be used to read and write OSI trace files to enable open loop simulations.

### Input and Output
The interface_name defines the name of a OSI message, which the FMU expects.\
The base_name defines the name used by the base simulator (CARLA).\
This differentiation allows free adjustments between FMUs and the base simulator.\
FMUs can send information by any not yet defined base_name between each other.

Additional information: [YAML Configuration](https://github.com/DLR-TS/CoSiMa/blob/master/Configuration.md)

## Parameters
| Parameter | Description |
| ------ | ------ |
| -v | Verbose outputs. |
| -p | Parallel model execution. |
| PATH.yaml | Path to Configuration file. |

## Used Libraries

cmake-conan 0.18\
gRPC 1.29.1\
protobuf 3.21.9\
open-simulation-interface 3.5.0

# Contacts

bjoern.bahn@dlr.de
danny.behnecke@dlr.de

This software was originally developed as part of SetLevel.
