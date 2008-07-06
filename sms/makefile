
export CPATH=../include:/usr/include/mysql

CC=g++ -g -Wall



RTCOMMON=../librtcommon/librtcommon.a

%.o:    %.cpp $(INCLUDES)
	$(CC) -c -g -o $@ $<

all: sms

sms: sms.cpp sms.h
	$(CC) -o sms sms.cpp -lmysqlclient -lnsl -lm -lz $(RTCOMMON)

clean:
	rm -fv sms *.o


