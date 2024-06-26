# IP-Project2
Internet Protocols Project 2 - CSC 573 Spring 2024

## Team Members

#### 1. Shivangi Chopra (schopra4)
#### 2. Sahil Purohit (spurohi2)


## Project Description
Understand the behavior of two protocols TCP Cubic and DCTCP when contention occurs in the network.

For the purpose of analysis, a dumbbell topology is created with 2 sender nodes, 2 receiver nodes and 2 routers in the middle. Sending nodes are connected to router 1, receiving nodes are connected to router 2 and both routers are connected to each other. All the links in this topology have bandwidth of 1 Gbps.

Analysis consists of 5 Experiments:

#### 1. Experiment 1.
Sender 1 sends 50 MB data to Receiver 1 using TCP Cubic protocol.

#### 2. Experiment 2.
Sender 1 sends 50 MB data to Receiver 1 and Sender 2 sends 50 MB data to Receiver 2 using TCP Cubic protocol.

#### 3. Experiment 3.
Sender 1 sends 50 MB data to Receiver 1 using DCTCP protocol.

#### 4. Experiment 4.
Sender 1 sends 50 MB data to Receiver 1 and Sender 2 sends 50 MB data to Receiver 2 using DCTCP protocol.

#### 5. Experiment 5.
Sender 1 sends 50 MB data to Receiver 1 using DCTCP Cubic protocol and Sender 2 sends 50 MB data to Receiver 2 using DCTCP Cubic protocol.


###### Special Note:
Project description mentions that we are understanding the behavior of differnet TCP protocols when contention happens. But project description does not mention anything about the parameters which can lead to contention like routet buffer size or queueing policy. Therefore, we have not taken any of these things under consideration during this project and hence all the results concludes to non-contention scenario.


#### Note: This code is tested on linux machine and not on Windows or Mac. So, we recommend to run the code on linux machine. All the instructions are for linux machine.

## Dependencies
This project uses ns3 simulator to run the network simulation scripts. To install ns3, please verify if following dependencies are installed. If not installed, install using the commands given:

1. Git
```
sudo apt install git-all
```

2. g++ compiler
```
sudo apt install g++
```

3. Python3
```
sudo apt install python3
```

4. cmake
```
sudo apt install cmake
```

## ns3 install and build
Once all the dependencies are installed, you can install the ns3 simulator. Follow these instructions:
```
wget https://www.nsnam.org/releases/ns-allinone-3.41.tar.bz2
tar xfj ns-allinone-3.41.tar.bz2
cd ns-allinone-3.41/ns-3.41
```

Configure the ns3 simulator using following command and then build
```
./ns3 configure --enable-examples --enable-tests
./ns3 build
```

Once the building is complete, you can run the test code to check your installtion and build:
```
./test.py
```

## Code Compilation and Execution
Copy the code file in the **scratch** folder of the ns3 simulator. (ns3 simulator folder name will be **ns-allinone-3.41/ns-3.41**)

To run the code use the following command:
```
cd ns-allinone-3.41/ns-3.41
./ns3 run scratch/tcp_schopra4_spurohi2.cc
```

These commands will compile and execute the code also. Once the command execution is complete, a CSV file named **tcp_schopra4_spurohi2.csv** will be generated. This file will be stored in the ns3 simulator folder. This file contains all the expected results generated by the code.

