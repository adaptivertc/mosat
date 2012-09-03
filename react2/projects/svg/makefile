
export CPATH=../../../include/

.PHONY : clean
.PHONY : all
.PHONY : so_files 

RTCOMMON=../../../librtcommon/librtcommon.a

CPP=g++ -Wall -Werror -std=c++0x

all: so_files gen_points gen_bar gen_tank gen_display


so_files: gen_so_makefile
	./gen_so_makefile
	cd plugins; make; cd ..

gen_points: gen_points.cpp
	$(CPP) -o gen_points gen_points.cpp

gen_bar: gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp
	$(CPP) -o gen_bar gen_bar.cpp gen_logo.cpp scales.cpp include_file.cpp

gen_so_makefile: gen_so_makefile.cpp
	$(CPP) -o gen_so_makefile gen_so_makefile.cpp

gen_tank: gen_tank.cpp
	$(CPP) -o gen_tank gen_tank.cpp

gen_display:  gen_display.cpp include_file.cpp gen_display.h react_svg.h
	$(CPP) gen_display.cpp include_file.cpp -ldl -rdynamic -o gen_display $(RTCOMMON)

dtest.svg: display.txt gen_display so_files
	./gen_display -f display.txt -d ./plugins/ -o dtest.svg -sim

install: 
	sudo cp ai.svg /var/www

depend:
	gcc -MM -MF depend gen_display.cpp

clean:
	rm -rv gen_points gen_bar gen_tank gen_so_makefile ./plugins/*.so gen_display

