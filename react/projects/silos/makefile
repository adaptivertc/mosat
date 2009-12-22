

all: temp weblogger divide webbackground gen_graph

temp: temp.cpp  
	g++ -Wall -o temp temp.cpp  
	../../pl out_temp_hum_sim.txt 

weblogger: weblogger.cpp
	g++ -Wall -lrt -o weblogger weblogger.cpp

webbackground: webbackground.cpp
	g++ -Wall -lrt -o webbackground webbackground.cpp

divide: divide.cpp
	g++ -Wall -g -o divide divide.cpp

gen_graph: gen_graph.cpp
	g++ -Wall -o gen_graph gen_graph.cpp

clean:
	rm -vf temp weblogger divide webbackground gen_graph

