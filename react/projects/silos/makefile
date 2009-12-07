

all: temp weblogger divide

temp: temp.cpp  
	g++ -Wall -o temp temp.cpp  
	../../pl out_temp_hum_sim.txt 

weblogger: weblogger.cpp
	g++ -Wall -o weblogger weblogger.cpp

clean:
	rm -vf temp weblogger divide

divide: divide.cpp
	g++ -Wall -g -o divide divide.cpp
