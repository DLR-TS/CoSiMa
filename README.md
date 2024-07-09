# OSTAR CoSimulationManager

CoSimulation Manager is part of [OSTAR](https://github.com/DLR-TS/OSTAR-Quickstart).
It connects multiple Co-Simulations.

# Installation Guide

## Linux

in root folder:
```sh
 # Preparation
 pip install conan==1.59.0
 # Checkout
 git clone https://github.com/DLR-TS/CoSiMa.git
 cd CoSiMa
 # Build
 mkdir build && cd build
 cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=false ..
 cmake --build . --target CoSimulationManager
 # Run
 cd bin
 ./CoSimulationManager
```

## Docker build

```sh
 git clone https://github.com/DLR-TS/CoSiMa.git
 cd CoSiMa
 docker build -t ostar:cosima .
```

# Run

| Imporant runtime parameter | Description |
| ------ | ------ |
| -v | Verbose outputs. |
| -sr | Synchronize with Carla Scenario Runner. |
| path.yml | Path to Configuration file. |

The complete list can be found [here](https://github.com/DLR-TS/OSTAR-Quickstart/tree/main/docu).

## Windows

Install [conan 1.x](https://conan.io/) \
Add conan.exe to PATH environment variable \
Open the directory in Visual Studio and use the cmake integration.

# Further information

Check out the OSTAR documentation at [OSTAR Quickstart](https://github.com/DLR-TS/OSTAR-Quickstart/tree/main/docu).

# Contacts

bjoern.bahn@dlr.de frank.baumgarten@dlr.de opensource-ts@dlr.de

This software was originally developed as part of SetLevel.
