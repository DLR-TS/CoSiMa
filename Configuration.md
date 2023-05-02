# YAML Configuration

In general the simulation configuration consists of a base simulator and several additional simulation model adapters.

## Base simulator

At the moment there is one base simulator (CARLA) as well as a dummy implemented.\
The base simulator will be contacted, if an input of a model is not filled by an output of an other model.
The dummy can be used to run a simulation without CARLA.

### Carla OSI Service Configuration
```
- simulator: CARLA
  carla_host: localhost
  carla_port: 2000
  client_host: localhost
  client_port: 51425
  delta: 0.03
  transaction_timeout: 30000
  do_step_timeout: 30000
  initialisation_timeout: 60000
```

carla_host and carla_port: host and port of CARLA instance  
client_host and client_port: host and port of Carla OSI Service  
delta: simulation stepsize in seconds  
transaction_timeout: timeout for gRPC getter and setter calls  
do_step_timeout: timeout for gRPC Do_Step calls  
initialisation_timeout: timeout for gRPC call for initialisation of Carla OSI Service

## Additional model adapters

### OSMP Service

```
- simulator: OSMP
  model: /Path/to/FMU.fmu
  prefix: "#test#"
  host: localhost
  port: 51426
  transaction_timeout: 5000
  do_step_timeout: 5000
  input:
      - {interface_name: OSMPSensorView, base_name: OSMPSensorViewGroundTruth, default_value: ""}
  output:
      - {interface_name: OSMPSensorData, base_name: OSMPSensorData, default_value: ""}
```

model: path where FMU is located, if not found by CoSiMa the OSMP Service will try to load the file directly. It can also be an OSI trace file as an input. 
prefix: prefix for OSI messages to distinguish it from other strings  
host and port: host and port of OSMP Service  
transaction_timout: timeout for gRPC getter and setter calls  
do_step_timeout: timeout for gRPC Do_Step calls  
input and output: list of inputs/outputs  
  interface_name: name of variable in modeldescription.xml of FMU, needs to be the same as OSI message name, can contain In and Out name prefix  
  base_name: name for matching the input and output inside CoSiMa  
  default_value: could define initial message
