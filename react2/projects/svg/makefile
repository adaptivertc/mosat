
export CPATH=../../../include/

RTCOMMON=../../../librtcommon/librtcommon.a

all: gen_points gen_bar gen_tank 

gen_points: gen_points.cpp
	g++ -g -std=c++0x -Wall -o gen_points gen_points.cpp

gen_bar: gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp
	g++ -g -Wall -o gen_bar gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp

gen_tank: gen_tank.cpp
	g++ -g -Wall -o gen_tank gen_tank.cpp

gen_display:  gen_display.cpp gen_display.h
	g++ -Wall gen_display.cpp -ldl -rdynamic -o gen_display $(RTCOMMON)

simple_rect.so: simple_rect.cpp gen_display.h
	g++ -Wall -shared -o simple_rect.so simple_rect.cpp
	cp simple_rect.so plugins/

simple_panel.so: simple_panel.cpp gen_display.h
	g++ -Wall -shared -o simple_panel.so simple_panel.cpp
	cp simple_panel.so plugins/

dtest.svg: display.txt gen_display gen_svg simple_rect.so simple_panel.so 
	./gen_display plugins/
	./gen_svg

install: 
	sudo cp ai.svg /var/www


clean:
	rm -rv gen_points gen_bar gen_tank
