Before you compile this, you must first copy the include files in trunk/mosat/include  to /usr/local/include, and then compile the library (librtcommon.so) in trunk/mosat/lib, and copy it to /usr/local/lib. Run ldconfig so that the library is recognized.

Then you can compile this by just running "make" from the command line. Then copy librtmodbus.so to /usr/local/lib and run ldconfig.

epending on the system, you may have to add the following line to 
/etc/ld.so.conf:

/usr/local/lib

Then run ldconfig again.


