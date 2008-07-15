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
	bool status;

public:
	void setData(const char *message, const char *number, const char *name, const char *group);
	const char *getMessage();
	const char *getNumber();
	const char *getName();
	const char *getGroup();
	void setSuccess(bool x);
	bool success();
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
	MYSQL_ROW row;
	MYSQL_RES *resGroups;
	MYSQL_ROW rowGroups;
	bool goodConfig;
	char gammuFile[150];
	smsMessage actualMessage;
	
	int sms_send_multiple(char *message);
	int sms_send_single(char *message);
	
	

public:
	sms(const char *host, const char *user, const char *password, const char *database, const char *smsdConfig, const char *receiveFile, const char *sendFile);
	~sms();
	bool sms_send(const char *message, const char *number);
	bool sms_send_member(const char *message, const char *name, const char *group);
	bool sms_send_group(const char *message, const char *group);
	bool sms_send_all(const char *message);
	void sms_prueba();
	smsMessage next_sms();
	smsMessage next_sms_number(const char *number);
	smsMessage next_sms_member(const char *name, const char *group);
	smsMessage next_sms_group(const char *group);
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
