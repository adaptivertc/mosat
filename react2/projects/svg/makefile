
export CPATH=../../../include/

.PHONY : clean
.PHONY : all
.PHONY : so_files 
.PHONY : doc 

RTCOMMON=../../../librtcommon/librtcommon.a

SILENT=-silent

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

gen_display:  gen_display.cpp doc_text.cpp doc_html.cpp include_file.cpp gen_display.h react_svg.h
	$(CPP) gen_display.cpp doc_text.cpp doc_html.cpp include_file.cpp -ldl -rdynamic -o gen_display $(RTCOMMON)

dtest.svg: display.txt gen_display so_files
	./gen_display $(SILENT) -f display.txt -d ./plugins/ -o dtest.svg -sim

install: 
	sudo cp ai.svg /var/www

depend:
	gcc -MM -MF depend gen_display.cpp

.PHONY: imasa gauge spray.slider slider svgs feed cool switch tina transfer spray

gauge:
	./gen_display $(SILENT) -f display.gauge.txt -o gauge.svg -sim

spray.slider:
	./gen_display $(SILENT) -f display.spray.slider.txt -o spray.slider.svg -sim

slider:
	./gen_display $(SILENT) -f display.slider.txt -o slider.svg -sim

doc:
	./gen_display $(SILENT) -doc -html -o svgdoc.html
	./gen_display $(SILENT) -doc -text -o svgdoc.txt

feed: 
	./gen_display $(SILENT) -f display.feedtanks.txt -o feed.svg -sim -popup

cool:
	./gen_display $(SILENT) -f display.coolingtower.txt -o cool.svg -sim -popup

switch:
	./gen_display $(SILENT) -f display.switch.txt -o switch.svg -sim

spray:
	./gen_display $(SILENT) -f display.spray.round.txt -o spray.svg -sim -popup

tina:
	./gen_display $(SILENT) -f display.tina.txt -o tina.svg -sim_file sim_tina.js -popup

transfer:
	./gen_display $(SILENT) -f display.transfer.txt -o transfer.svg -sim

imasa:
	./gen_display $(SILENT) -f display.imasa.txt -o imasa.svg -sim

pipes:
	./gen_display $(SILENT) -f display.pipes.txt -o pipes.svg -sim

pipes2d:
	./gen_display $(SILENT) -f display.pipes2d.txt -o pipes2d.svg -sim

clean:
	rm -rv gen_points gen_bar gen_tank gen_so_makefile ./plugins/*.so gen_display

svgs: feed cool spray transfer tina switch imasa pipes pipes2d gauge

