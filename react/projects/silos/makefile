

all: temp weblogger divide webbackground

temp: temp.cpp  
	g++ -Wall -o temp temp.cpp  
	../../pl out_temp_hum_sim.txt 

weblogger: weblogger.cpp
	g++ -Wall -o weblogger weblogger.cpp

webbackground: webbackground.cpp
	g++ -Wall -o webbackground webbackground.cpp

divide: divide.cpp
	g++ -Wall -g -o divide divide.cpp

clean:
	rm -vf temp weblogger divide webbackground

