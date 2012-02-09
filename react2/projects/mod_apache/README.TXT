
1) Install apache!
   sudo apt-get install apache2 apache2-dev apache2-doc

2) compile with:

        apxs2 -c mod_helloworld.c

3) load into Apache with:

        sudo apxs2 -ie mod_helloworld.la

4) Create file /etc/apache2/mods-available/helloworld.conf (5 lines) to run this module for ALL subdiretories of /helloworld:

<IfModule mod_helloworld.c>
  <Location /helloworld>
    SetHandler helloworld
  </Location>
</IfModule>


5) Create the file /etc/apache2/mods-available/helloworld.load (one line) to load the module:

LoadModule helloworld_module /usr/lib/apache2/modules/mod_helloworld.so

6) Enable the module (basically creates two symbolic links):

	a2enmod helloworld

6) Restart Apache:

	/etc/init.d/apache2 restart


