

all: main gen_sqlite out_DI out_AI react_DI.sqlite react_AI.sqlite read_dat

main: main.cpp 
	g++ -g -Wall -o main -lsqlite3 main.cpp

gen_sqlite: gen_sqlite.cpp
	g++ -g -Wall -o gen_sqlite gen_sqlite.cpp ../../librtcommon/librtcommon.a  

read_dat: read_dat.cpp
	g++ -g -Wall -o read_dat read_dat.cpp ../../librtcommon/librtcommon.a

out_DI.cpp: gen_sqlite ../dbfiles/di.config ../dbfiles/di.dat
	./gen_sqlite ../dbfiles/di.config ../dbfiles/di.dat

out_DI: out_DI.cpp
	g++ -g -Wall -o out_DI -lsqlite3 out_DI.cpp ../../librtcommon/librtcommon.a

out_AI.cpp: gen_sqlite ../dbfiles/ai.config ../dbfiles/ai.dat
	./gen_sqlite ../dbfiles/ai.config ../dbfiles/ai.dat

out_AI: out_AI.cpp
	g++ -g -Wall -o out_AI -lsqlite3 out_AI.cpp ../../librtcommon/librtcommon.a

react_DI.sqlite: out_DI
	rm -f react_DI.sqlite
	./out_DI

react_AI.sqlite: out_AI
	rm -f react_AI.sqlite
	./out_AI

clean: 
	rm -fv main gen_sqlite out_DI out_AI out_DI.cpp out_AI.cpp read_dat
