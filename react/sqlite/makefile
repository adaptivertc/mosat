
export CPATH=../../include

RTCOMMON=../../librtcommon/librtcommon.a


all: main gen_sqlite out_di out_ai react_di.sqlite react_ai.sqlite read_dat

main: main.cpp 
	g++ -g -Wall -o main -lsqlite3 main.cpp

gen_sqlite: gen_sqlite.cpp
	g++ -g -Wall -o gen_sqlite gen_sqlite.cpp $(RTCOMMON) 

read_dat: read_dat.cpp
	g++ -g -Wall -o read_dat read_dat.cpp $(RTCOMMON) 

out_di.cpp: gen_sqlite ../dbfiles/di.config ../dbfiles/di.dat
	./gen_sqlite ../dbfiles/di.config ../dbfiles/di.dat

out_di: out_di.cpp
	g++ -g -Wall -o out_di -lsqlite3 out_di.cpp $(RTCOMMON) 

out_ai.cpp: gen_sqlite ../dbfiles/ai.config ../dbfiles/ai.dat
	./gen_sqlite ../dbfiles/ai.config ../dbfiles/ai.dat

out_ai: out_ai.cpp
	g++ -g -Wall -o out_ai -lsqlite3 out_ai.cpp $(RTCOMMON)  

react_di.sqlite: out_di
	rm -f react_di.sqlite
	./out_di

react_ai.sqlite: out_ai
	rm -f react_ai.sqlite
	./out_ai

xx: gen_one_fns.cpp outmain.cpp insert_from_dat.cpp
	g++ -o xx -g -Wall gen_one_fns.cpp outmain.cpp insert_from_dat.cpp -lsqlite3 $(RTCOMMON)

clean: 
	rm -fv main gen_sqlite out_di out_ai out_di.cpp out_ai.cpp read_dat react_def.sqlit  gen_one_fns.cpp outmain.cpp
