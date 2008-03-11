This is the first release of AdaptiveRTC Viewer

It is developed on python using Gecko (layout engine).

REQUERIMENTS:

python
gtk
pygtk
python-gnome-extras  (it will bring gtkmozembed module)

NOTE:

Nowadays there are a bug on actual distributions that involves gecko and
pygtkmozembed, and the library search path:

The distributions that has that problem AFAIK are:
opensuse 10.3 and Ubuntu 7.10 (gutsy)


You have to set LD_LIBRARY_PATH and MOZILLA_FIVE_HOME so pygtkmozembed can
look for the mozilla gtkmozembed library.

To do that you have to follow these steps


opensuse:

$ rpm -ql python-gnome-extras  | grep gtkmozembed.so
/usr/lib/python2.5/site-packages/gtk-2.0/gtkmozembed.so

ldd /usr/lib/python2.5/site-packages/gtk-2.0/gtkmozembed.so | grep libgtkembed
        libgtkembedmoz.so => /usr/lib/xulrunner-1.8.1.4/libgtkembedmoz.so (0xb7f93000)

export LD_LIBRARY_PATH=/usr/lib/xulrunner-1.8.1.4
export MOZILLA_FIVE_HOME=/usr/lib/xulrunner-1.8.1.4


ubuntu 7.10 (gutsy)


$ dpkg -L python-gnome2-extras | grep gtkmoz
/usr/lib/python-support/python-gnome2-extras/python2.5/gtk-2.0/gtkmozembed.so
/usr/lib/python-support/python-gnome2-extras/python2.4/gtk-2.0/gtkmozembed.so

ldd /usr/lib/python-support/python-gnome2-extras/python2.5/gtk-2.0/gtkmozembed.so  | grep libgtkembedmoz
        libgtkembedmoz.so => /usr/lib/firefox/libgtkembedmoz.so (0xb7fc2000)


export LD_LIBRARY_PATH=/usr/lib/firefox
export MOZILLA_FIVE_HOME=/usr/lib/firefox

then you are able to run ./adaptivertc_viewer.py without any problems


NOTES: (You have to have apache running and trainpos directory pointing to
trainpos)

NOTE2: I have to be more specific on this documentation.... :S
