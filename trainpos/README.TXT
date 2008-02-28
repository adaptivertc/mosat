This is trainpos, a program to monitor the position of trains.

Installation:

On any Linux distribution, you first need to compile and install the library rtcommon. Under trunk, go to the directory lib and follow the directions in the README.TXT file.

Following this, you need to compile this project. In a terminal, change to this directory, and justy type "make".

To run:

You need to open two terminals in this directory. In the first one, runt the program "main".

Only after main is running, run "line_sim". This program will simulate roughly 1.5 hours of operation free running, starting at 5:00 am. It will then slow down and simulate at 10x real-time. If you want to simulate in real-time mode (very, very boring, but what an operator would see) then use the -rt option.

To view the displays, open FireFox, and open the following two files in this directory: line1.html line1_table.html. One is a graphical display and you will see trains moving, the other is tabualr.

---------------------------

Design issues.

How to select the correct timetable. We really need to make the selection of the timetable to use a function of the location, since each country has different holidays and circumstances that affect how many schedules they will have and how to select them. Furthere, there may be special events, so, there must be a way to override the selection of timetables. For now, we are taking the easy way out, and just simulating one schedule type for each day.

We need to completely separate the display generation from the position calculations.

The file format for the sections file. We need a generic file format that will work for all locations if possible. Since all light rail systems have stations, we can define the sections based on passenger stations. Then we will allow 0 or more sensors per section. The current assumption is exactly two sensors per section.

We also need to be able to handle very soon the differences between having inductive sensors, and track circuits that give a "presense" signal. The two could be treated the same, but, it might be good to use the extra information we get from "presense" iformation. At the very least, we need to have special alarms for the types of failures possible for track circuits. With track circuits, you have the invariant that when you lose the presense in one segment, you must gain the presense in the next. We also know other characteristics, such that you should never gain lose presense if the train has not been in the segment long enough to reach the end. At SITEUR, they are considering a special monitoring system for the track circuit hardware.

So, the question is: is it better to have a sub-system to convert presense information into crossing information, and do alarm checking, then pass that information to the existing modules that only understand crossing events, or, should we just replace the existing modules?

For the radio systems, we WILL just replace the modules, so probably better to just replace the module too.




