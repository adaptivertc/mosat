# Makefile for react.

all:

install:
	mkdir -p /usr/local/etc/react2/web
	cp webserver.js /usr/local/etc/react2/web/
	cp -r ../svg /usr/local/etc/react2/

uninstall:
	rm -rf /usr/lcoal/etc/react2/web
	rm -rf /usr/local/etc/react2/svg

install-ubuntu-inits:
	cp init/ubuntu/init/react2web.conf /etc/init/
	cp init/ubuntu/init.d/react2web /etc/init.d/
	chmod a+x /etc/init.d/react2web
	update-rc.d react2web defaults

uninstall-ubuntu-inits:
	update-rc.d react2web remove
	rm /etc/init/react2web.conf
	rm /etc/init.d/react2web

