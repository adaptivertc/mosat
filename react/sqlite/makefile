
export CPATH=../../include

RTCOMMON=../../librtcommon/librtcommon.a


all: main gen_sqlite react_def.sqlite read_dat

main: main.cpp 
	g++ -g -Wall -o main -lsqlite3 main.cpp

gen_sqlite: gen_sqlite.cpp
	g++ -g -Wall -o gen_sqlite gen_sqlite.cpp $(RTCOMMON) 

outmain.cpp gen_one_fns.cpp: gen_sqlite
	./gen_sqlite ../dbfiles/

read_dat: read_dat.cpp
	g++ -g -Wall -o read_dat read_dat.cpp $(RTCOMMON) 

react_def.sqlite: gen_react_db
	./gen_react_db

gen_react_db: gen_one_fns.cpp outmain.cpp insert_from_dat.cpp
	g++ -o gen_react_db -g -Wall gen_one_fns.cpp outmain.cpp insert_from_dat.cpp -lsqlite3 $(RTCOMMON)

clean: 
	rm -fv main gen_sqlite out_di out_ai out_di.cpp out_ai.cpp read_dat react_def.sqlite gen_one_fns.cpp outmain.cpp
