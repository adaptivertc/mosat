
export CPATH=../../../include/

RTCOMMON=../../../librtcommon/librtcommon.a

all: gen_points gen_bar gen_tank gen_display dtest.svg 

gen_points: gen_points.cpp
	g++ -g -std=c++0x -Wall -o gen_points gen_points.cpp

gen_bar: gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp
	g++ -g -Wall -o gen_bar gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp

gen_tank: gen_tank.cpp
	g++ -g -Wall -o gen_tank gen_tank.cpp

gen_display:  gen_display.cpp include_file.cpp gen_display.h react_svg.h simple_panel.so simple_rect.so imasa_logo.so background.so svg_header.so
	g++ -Wall gen_display.cpp include_file.cpp -ldl -rdynamic -o gen_display $(RTCOMMON)

simple_rect.so: simple_rect.cpp gen_display.h
	g++ -Wall -shared -o simple_rect.so simple_rect.cpp
	cp simple_rect.so plugins/

background.so: background.cpp gen_display.h
	g++ -Wall -shared -o background.so background.cpp
	cp background.so plugins/

imasa_logo.so: imasa_logo.cpp gen_display.h
	g++ -Wall -shared -o imasa_logo.so imasa_logo.cpp
	cp imasa_logo.so plugins/

simple_tank.so: simple_tank.cpp gen_display.h
	g++ -Wall -shared -o simple_tank.so simple_tank.cpp
	cp simple_tank.so plugins/

simple_contact.so: simple_contact.cpp gen_display.h
	g++ -Wall -shared -o simple_contact.so simple_contact.cpp
	cp simple_contact.so plugins/

simple_pipe.so: simple_pipe.cpp gen_display.h
	g++ -Wall -shared -o simple_pipe.so simple_pipe.cpp
	cp simple_pipe.so plugins/

simple_panel.so: simple_panel.cpp gen_display.h
	g++ -Wall -shared -o simple_panel.so simple_panel.cpp
	cp simple_panel.so plugins/

only_text.so: only_text.cpp gen_display.h
	g++ -Wall -shared -o only_text.so only_text.cpp
	cp only_text.so plugins/

pendulo.so: pendulo.cpp gen_display.h
	g++ -Wall -shared -o pendulo.so pendulo.cpp
	cp pendulo.so plugins/

svg_header.so: svg_header.cpp gen_display.h
	g++ -Wall -shared -o svg_header.so svg_header.cpp
	cp svg_header.so plugins/

insert_svg.so: insert_svg.cpp gen_display.h
	g++ -Wall -shared -o insert_svg.so insert_svg.cpp
	cp insert_svg.so plugins/

dtest.svg: display.txt gen_display simple_rect.so simple_panel.so only_text.so pendulo.so insert_svg.so simple_pipe.so simple_tank.so imasa_logo.so simple_contact.so background.so svg_header.so
	./gen_display plugins/

install: 
	sudo cp ai.svg /var/www


clean:
	rm -rv gen_points gen_bar gen_tank *.so ./plugins/*.so gen_display dtest.svg
