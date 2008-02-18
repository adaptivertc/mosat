To run react, you must first copy the include files in trunk/mosat/include  to /usr/local/include, and then compile the library (librtcommon.so) in trunk/mosat/lib, and copy it to /usr/local/lib. 

You also need to copy ai.so from the react directory to /usr/local/lib

Run ldconfig so that the libraries are recognized.

Then you can compile react by simply typing "make" from the comand line in this directory.

To see an example, run the following:

./react control_pumps.txt

Then, open FireFox, and open trunk/react/html/pump_diagrm.html

You will see the simulated animation of a pumping station.
