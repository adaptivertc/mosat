This is a Software for monitoring the departures of a Light Rail Train.

To have this software running you need to have some specific hardware such as:
At least 1 RTU (Remote Terminal Unit)
...
<List of hardware>
...

Also you need some extra software you might not have:
gcc
make
powerpc-linux-gcc
...
<list of software>
...

When you have this requirements the you are ready to start.

The first step is to copy the include files in /usr/local/include or anywhere
your compiler can find the libraries, then you're ready to run make on the lib
directory, this is in order to obtain the library librtcommon.so. When this
library is generated you need to copy it into /usr/local/lib, then append this
line to the file /etc/ld.so.conf:
/usr/local/lib and run ldconfig. Ever command but the make are meant to be
runned as root.

After having the last steps done you can 

