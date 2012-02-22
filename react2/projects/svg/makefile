
export CPATH=../../../include/

RTCOMMON=../../../librtcommon/librtcommon.a

all: gen_points gen_bar gen_tank gen_display dtest.svg 

gen_points: gen_points.cpp
	g++ -g -std=c++0x -Wall -o gen_points gen_points.cpp

gen_bar: gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp
	g++ -g -Wall -o gen_bar gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp

gen_tank: gen_tank.cpp
	g++ -g -Wall -o gen_tank gen_tank.cpp

gen_display:  gen_display.cpp include_file.cpp gen_display.h react_svg.h simple_panel.so simple_rect.so
	g++ -Wall gen_display.cpp include_file.cpp -ldl -rdynamic -o gen_display $(RTCOMMON)

simple_rect.so: simple_rect.cpp gen_display.h
	g++ -Wall -shared -o simple_rect.so simple_rect.cpp
	cp simple_rect.so plugins/

simple_panel.so: simple_panel.cpp gen_display.h
	g++ -Wall -shared -o simple_panel.so simple_panel.cpp
	cp simple_panel.so plugins/

only_text.so: only_text.cpp gen_display.h
	g++ -Wall -shared -o only_text.so only_text.cpp
	cp only_text.so plugins/

insert_svg.so: insert_svg.cpp gen_display.h
	g++ -Wall -shared -o insert_svg.so insert_svg.cpp
	cp insert_svg.so plugins/

dtest.svg: display.txt gen_display simple_rect.so simple_panel.so only_text.so insert_svg.so 
	./gen_display plugins/

install: 
	sudo cp ai.svg /var/www


clean:
	rm -rv gen_points gen_bar gen_tank *.so ./plugins/*.so gen_display dtest.svg
