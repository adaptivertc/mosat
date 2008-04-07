This is trainpos, a program to monitor the position of trains.

Installation:

On any Linux distribution, you just need to compile from the root (trunk), and it will make the libraries, then the applications in order.
 
To run:

You need to open two terminals in this directory. In the first one, runt the program "onboard".

Only after onboard is running, run "line_sim". This program will simulate roughly 1.5 hours of operation free running, starting at 5:00 am. It will then slow down and simulate at 10x real-time. If you want to simulate in real-time mode (very, very boring, but what an operator would see) then use the -rt option.

To view the displays, open FireFox, and open the following two files in this directory: line1.html line1_table.html. One is a graphical display and you will see trains moving, the other is tabualr.
--------------------------

Configuration

To configure trainpos, you start by editing the main configuration file: "tpconfig.txt", where each line has the form key,value. You must define as a minimum: SECTIONS_FILE, TIMETABLE_FILE, DISPLAY_FILE. An example is given below:

SECTIONS_FILE,data/india_sections.txt
TIMETABLE_FILE,data/india_weekdays.txt
DISPLAY_FILE,display_info.txt

Next, you must create the sections file, where each line has the form: 

<station name><tab><time to travel section><tab><time to first sensor><tab><time to second sensor>

Here are some example lines:

Chennai Beach   226     4       222
Chennai Fort    138     4       134
Park Town       125     4       121
Chintadripet    108     4       104
Chepauk 130     4       126
Tiruvallikkeni  108     4       104
Light House     163     4       159
Tirumayilai     135     4       131
Mandaiveli      120     4       116
Greenways Road  132     4       128

You create the sections up and back, with the last section starting at the last section before arriving back. In the case
above, the last section would be "Chennai Fort".

Next, you must create the timetable file which has the form:

<train name or id><tab><time-HH:MM:SS>

BVL1    6:05:00
BVL3    6:20:00
BVL5    6:40:00
BVL7    6:55:00
BVL9    7:10:00
BVL11   7:20:00
BVL13   7:35:00
BVL15   7:50:00
BVL17   8:00:00
BVL19   8:10:00
BVL21   8:20:00
BVL23   8:30:00
BVL25   8:45:00
BVL27   9:00:00

The times must be strictly increasing, starting with the first departure in the morning. This file is used by BOTH the simulator, and the train position monitor. The simulator uses it to simulate departures, and the position monitor uses it to match a train with a detected departure, and then display the train id.

The final configuration file is for the graphical display. It has the following form for each line:

<background image>,<square image>,<square image for unexpected>, <output html file>,X1,X2,Y1,Y2,<number of sections>

dia/CHENNAIBEACH_VELACHERY.png,dia/square12x12.png,dia/square12x12.png,Line1.html,24,968,63,94,32

Important: the number of sections MUST match with the timetable file.

(X1,Y1) is the location of the upper left square, and (X2,Y2) is the location of the lower right square. With this information, trainpos can calculate where to put the square in units of 25% of the travel between any two stations.

---------------------------

Design issues.

How to select the correct timetable. We really need to make the selection of the timetable to use a function of the location, since each country has different holidays and circumstances that affect how many schedules they will have and how to select them. Furthere, there may be special events, so, there must be a way to override the selection of timetables. For now, we are taking the easy way out, and just simulating one schedule type for each location.

We need to completely separate the display generation from the position calculations. (this is in process)

The file format for the sections file. We need a generic file format that will work for all locations if possible. Since all light rail systems have stations, we can define the sections based on passenger stations. Then we will allow 0 or more sensors per section. The current assumption is exactly two sensors per section.

We also need to be able to handle very soon the differences between having inductive sensors, and track circuits that give a "presense" signal. The two could be treated the same, but, it might be good to use the extra information we get from "presense" iformation. At the very least, we need to have special alarms for the types of failures possible for track circuits. With track circuits, you have the invariant that when you lose the presense in one segment, you must gain the presense in the next. We also know other characteristics, such that you should never gain lose presense if the train has not been in the segment long enough to reach the end. At SITEUR, they are considering a special monitoring system for the track circuit hardware.

So, the question is: is it better to have a sub-system to convert presense information into crossing information, and do alarm checking, then pass that information to the existing modules that only understand crossing events, or, should we just replace the existing modules?

For the radio systems, we WILL just replace the modules, so probably better to just replace the module too.

