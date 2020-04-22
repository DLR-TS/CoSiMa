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
- git clone https://github.com/qtronic/fmusdk.git
- git clone https://github.com/jbeder/yaml-cpp.git


CMake
and build CoSimulationManager
