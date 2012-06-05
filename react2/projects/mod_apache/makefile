

.PHONY: mod install restart clean

react:
	apxs2 -c mod_react.c

helloworld: 
	apxs2 -c mod_helloworld.c

install:
	sudo apxs2 -ie mod_helloworld.la
	sudo apxs2 -ie mod_react.la

restart:
	sudo /etc/init.d/apache2 restart


clean:
	rm -rfv mod_helloworld.la mod_helloworld.lo mod_helloworld.slo .libs 
