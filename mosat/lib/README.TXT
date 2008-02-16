This is the library that is common for most AdaptiveRTC programs. You must first copy all of the include files in ../include to /usr/local/include if that has not already been done. To install this library, first run "make" in this directory. Then, as root, copy the file librtcommon.so to /usr/local/lib. Finally, as root, run ldconfig. You are now ready to use the library. 

Depending on the system, you may have to add the following line to /etc/ld.so.conf:

/usr/local/lib

Then run ldconfig again.


