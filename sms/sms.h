#include <mysql.h>
#include <stdio.h>
#include "rtcommon.h"
#include "arg.h"
#include <string.h>
#include <queue>
#include <math.h>

#define MSGMAXLEN 161
#define MULTIMSGMAXLEN 481
#define NUMMAXLEN 19
#define NAMEMAXLEN 31
#define LISTMAXLEN 50

using namespace std;

class smsMessage
{
private:
	char message[MULTIMSGMAXLEN];
	char number[NUMMAXLEN];
	char name[NAMEMAXLEN];
	char group[NAMEMAXLEN];

public:
	smsMessage(const char *message, const char *number, const char *name, const char *group);
	char *getMessage();
	char *getNumber();
	char *getName();
	char *getGroup();
};

class smsSplit
{
private:
	short int count;
	short int actualChar;
	short int actualMessage;
	char multiMsg[MULTIMSGMAXLEN];
	char msg[MSGMAXLEN];
public:
	smsSplit(const char *text);
	short int getCount();
	const char *getNextMsg();
};

class sms
{

private:
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_RES *resGroups;
	MYSQL_ROW rowGroups;
	
	bool goodConfig;
	
	int sms_send_multiple(char *message);
	int sms_send_single(char *message);
	
	

public:
	sms(const char *host, const char *user, const char *password, const char *database, const char *receiveFile, const char *sendFile);
	~sms();
	int sms_send(const char *message, const char *number);
	bool sms_send_member(const char *message, const char *name, const char *group);
	int sms_send_group(const char *message, const char *group);
	int sms_send_all(const char *message);
	smsMessage nextMessage();
	

};

class reader
{

private:
	char header[NAMEMAXLEN];
	char token[NAMEMAXLEN];
	char temp;
	FILE *fp;
	bool goodConfig;
	bool finish;

public:
	void readerInit(const char *path);
	const char *nextHeader();
	const char *nextToken();
	bool state();
	bool finished();
};
