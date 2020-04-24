# CoSiMa

CoSimulationManager for SETLevel4to5

Installation Guide

in lib folder:
-  git clone https://github.com/protocolbuffers/protobuf.git
	- cd protobuf
	- git checkout tags/v3.11.4
	- git submodule update --init --recursive
	- cmake ausführen (Ziel build2) und bauen
		-libprotobuf
		-libprotobuf-lite
		-libprotoc
		-protoc
- git clone https://github.com/OpenSimulationInterface/open-simulation-interface.git
	- cd open-simulation-interface
	- git checkout tags/v3.2.0
	- cmake
	(- open-simulation-interface von dll auf lib umstellen
		- alle können kompiliert werden)
- git clone https://github.com/jbeder/yaml-cpp.git
- git clone https://github.com/modelon-community/fmi-library

Tests

- git clone https://github.com/catchorg/Catch2.git
	- cd Catch2
	- git checkout tags/v2.12.1
- git clone https://github.com/eranpeer/FakeIt.git
	- cd FakeIt
	- git checkout tags/2.0.5

CMake
and build CoSimulationManager
