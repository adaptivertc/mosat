To make al of the projects, just run "make" from this directory, and it will visit all of the directories in the correct order and make each one of them. You might have to install things such as ncurses. Here is a list of things you might need to install, with the commands for Ubuntu/Debian:

apt-get install manpages-dev
apt-get install gcc
apt-get install g++
apt-get install make

apt-get install libncurses5
apt-get install libncurses5-dev
apt-get install glibc-doc

There are basically six important projects here:

1) Mosat, wich is a departure monitoring system currently in use at SITEUR (Light Rail System Guadalajara, Mexico). It keeps track of the departures at the extreme of line 1 and line 2, and could be easily adapted to any light rail system. With this, the operators in central control will know the departures in real-time, and how man seconds early/late each departure was. There are also monthly reports generated for management.

2) React. React is a scalable control engine that can be run on anything from a small single board computer running Linux, to a larger multi-processor server. On a single board computer, it turns it into an RTU, with AI, DI, AO, DO, PID, data logging, control scripts, etc. It is currently being ported to Technologic Systems SBCs, starting with TS7400 and TS7300, and will be ported to more (write us with port requests, we are all ears). It is currently being used for monitoring pumping stations on the light rail system, and controlling laboratory instruments. It is currently being implemented in a control project for a small hotel with solar hot water systems.

3) Trainpos. This is meant as an add-on for traditional light rail control systems that either employ axle counters, or track circuits. If you have all of the signals brought into your central control, to light up a control panel, you can hook it up directly to get a ton more information, such as constanly having an estimated position of each train (corrected each time a sensor is crossed or section entered), seconds early/late for each train in the system, time between all trains, etc. There is a plug-in architecture, so you can write you own custom alarming, and/or performance monitoring, etc.  

4) Onboard. This is software to run on each train, and monitor actual speed and compare it to desired speed obtained from velocity profiles. It can be programmed to first warn a driver for over speed, and eventually stop the train if warnings are ignored. It could of course implement automatic train control, if so desired. We have purchased 22 900 MHz radios, and will do pilot tests with radios as well. The radios will be used to create what we would call a "poor mans CBTC" to start, and will likely develop into a more complete CBTC solution over time. It will also be used to communicate the state of approaching crossing guards to the operator, and also communicate the the detection of vehicles and/or pedestrians entering the intersection.

5) ClassicTControl. This was our first stab at a train control project, and uses N-Version programming with a voting algorithm such that all algorithms have to have errors before dangerous situations are allowed. It also uses a control algorithm running concurrently with the safety algorithms. 

6) Modbus library for modbus TCP/IP. This currently implements Modbus TCP/IP, both client and server, but we are also working to implement Modbus serial and RTU, bridges for TCP/IP to serial, etc, a complete package. And, of course integration with React. With these libraries, you can create your own modbus server. There is an example server simulator.