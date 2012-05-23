
export CPATH=../../../include/

.PHONY : clean
.PHONY : all
.PHONY : so_files 

RTCOMMON=../../../librtcommon/librtcommon.a


all: so_files gen_points gen_bar gen_tank gen_display


so_files: gen_so_makefile
	./gen_so_makefile
	cd plugins; make; cd ..

gen_points: gen_points.cpp
	g++ -g -std=c++0x -Wall -o gen_points gen_points.cpp

gen_bar: gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp
	g++ -g -Wall -o gen_bar gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp

gen_so_makefile: gen_so_makefile.cpp
	g++ -Wall -o gen_so_makefile gen_so_makefile.cpp

gen_tank: gen_tank.cpp
	g++ -g -Wall -o gen_tank gen_tank.cpp

gen_display:  gen_display.cpp include_file.cpp gen_display.h react_svg.h
	g++ -Wall gen_display.cpp include_file.cpp -ldl -rdynamic -o gen_display $(RTCOMMON)

dtest.svg: display.txt gen_display so_files
	./gen_display -f display.txt -d ./plugins/ -o dtest.svg -sim

install: 
	sudo cp ai.svg /var/www

clean:
	rm -rv gen_points gen_bar gen_tank gen_so_makefile ./plugins/*.so gen_display
