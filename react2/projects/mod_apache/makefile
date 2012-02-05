


mod: 
	apxs2 -c mod_helloworld.c
	sudo apxs2 -ie mod_helloworld.la

restart:
	sudo /etc/init.d/apache2 restart


clean:
	rm -rfv mod_helloworld.la mod_helloworld.lo mod_helloworld.slo .libs 
