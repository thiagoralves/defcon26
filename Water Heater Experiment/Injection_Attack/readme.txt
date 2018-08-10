This code was used to inject modbus commands to the water heater experiment. The injected commands kept the system in manual mode and prevented the operator from turning off the heater

To compile: g++ injection_attack.cpp -o injection_attack -pthread
To run: ./injection_attack -h [host ip] -f [frequency of the messages]