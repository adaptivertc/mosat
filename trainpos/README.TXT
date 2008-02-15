This is trainpos, a program to monitor the position of trains.

Installation:

On any Linux distribution, you first need to compile and install the library rtcommon. Under trunk, go to the directory lib and follow the directions in the README.TXT file.

Following this, you need to compile this project. In a terminal, change to this directory, and justy type "make".

To run:

You need to open two terminals in this directory. In the first one, runt the program "main".

Only after main is running, run "line_sim". This program will simulate roughly 1.5 hours of operation free running, starting at 5:00 am. It will then slow down and simulate at 10x real-time. If you want to simulate in real-time mode (very, very boring, but what an operator would see) then use the -rt option.

To view the displays, open FireFox, and open the following two files in this directory: line1.html line1_table.html. One is a graphical display and you will see trains moving, the other is tabualr.
