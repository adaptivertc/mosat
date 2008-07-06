/*
The following example program (in file prog.c) should be compiled with this Unix command:

     gcc -I/usr/include/mysql sms.c -lmysqlclient -lnsl -lm -lz -o sms

*/

#include "sms.h"

void reader::readerInit(const char *path)
{
	 fp = fopen(path, "r");
	 if(fp)
	 {
	 	goodConfig = true;
	 	finish = false;
	 }
	 else
	 {
	 	goodConfig = false;
	 	finish = true;
	 }
	 
	 temp = '@';
}

const char* reader::nextHeader()
{

	int i = 0;
	
	while(!feof(fp) && temp != '[')
		if(fgetc(fp) == '[')
			break;
	
	if(feof(fp))
	{
		finish = true;
		goodConfig = false;
		return "@";
	}
	
	while(!feof(fp))
	{
		temp = fgetc(fp);
		if(temp == ']')
		{
			header[i] = '\0';
			return header;
		}
		else
		{
			header[i] = temp;
		}
		i++;
	}
	
	finish = true;
	goodConfig = false;
	
	return "@";
}
const char* reader::nextToken()
{

	int i = 0;
	
	if(temp != '[')
	{	while(!feof(fp) && temp != '|' && temp != '[')
		{

			temp = fgetc(fp);
			if(temp == '|')
				break;
		}
		
		if(temp == '[')
			return "@";
		
		if(feof(fp))
		{
			finish = true;
			return "@";
		}
	
		while(!feof(fp))
		{
			temp = fgetc(fp);
			if(temp == '[')
				return "@";
			if(temp == '|')
			{
				token[i] = '\0';
				return token;
			}
			else
			{
				token[i] = temp;
			}
			i++;
		}
	
		finish = true;
		goodConfig = false;
	
		return "@";
	}
	
	return "@";
}

bool reader::finished()
{
	return finish;
}

bool reader::state()
{
	return goodConfig;
}

smsSplit::smsSplit(const char *text)
{
	safe_strcpy(multiMsg,text,sizeof(multiMsg));
	if(strlen(text) > MULTIMSGMAXLEN)
	{
		printf("Mensaje demasiado largo");
		count = 0;
	}
	else
		if(strlen(text) < 1)
			printf("Mensaje demasiado corto");
		else
		{
			actualChar = 0;
			actualMessage = 0;
			count = ceil(strlen(multiMsg)/155);
		}
}

short int smsSplit::getCount()
{
	return count;
}

const char *smsSplit::getNextMsg()
{
	int i;
	for(i = 0; i < 155; i++)
	{
		msg[i] = multiMsg[actualChar];
		if(msg[i] == '\0')
			return msg;
		actualChar++;
	}
	msg[i] = '\0';
	return msg;
}

sms::~sms()
{
	mysql_close(conn);
}


sms::sms(const char *host, const char *user, const char *password, const char *database, const char *receiveFile, const char *sendFile)
{

	char query[300];
	char tempHeader[NAMEMAXLEN];
	char tempNumber[NUMMAXLEN];
	char tempName[NAMEMAXLEN];
	goodConfig = true;
	
	
	conn = mysql_init(NULL);
   	
   	/* Connect to database */
   	if (!mysql_real_connect(conn, host, user, password, database, 0, NULL, 0))
   	{
      		fprintf(stderr, "%s\n", mysql_error(conn));
      		goodConfig = false;
      	}
      	
      	if (mysql_query(conn, "DELETE FROM receive"))
      	{
        	fprintf(stderr, "%s\n", mysql_error(conn));
      		goodConfig = false;
   	}
   	
   	if (mysql_query(conn, "DELETE FROM send"))
      	{
        	fprintf(stderr, "%s\n", mysql_error(conn));
      		goodConfig = false;
   	}

      	reader config;
      	
      	config.readerInit(receiveFile);
      	
      	while(config.state() && !config.finished())
      	{
      		safe_strcpy(tempHeader,config.nextHeader(),sizeof(tempHeader));

      		while(config.state() && !config.finished())
      		{
      			safe_strcpy(query,"insert into receive (number,name,type) values ('",sizeof(query));
      			safe_strcpy(tempNumber,config.nextToken(),sizeof(tempNumber));
      			safe_strcpy(tempName,config.nextToken(),sizeof(tempName));
      			
      			if(strcmp(tempNumber,"@") == 0)
      				break;

      			safe_strcat(query,tempNumber,sizeof(query));
      			safe_strcat(query,"','",sizeof(query));
      			safe_strcat(query,tempName,sizeof(query));
      			safe_strcat(query,"','",sizeof(query));
      			safe_strcat(query,tempHeader,sizeof(query));
      			safe_strcat(query,"');",sizeof(query));

      			if (mysql_query(conn,query))
      			{
      				fprintf(stderr, "%s\n", mysql_error(conn));
      				goodConfig = false;
   			}
      		}
      	}
      	
      	config.readerInit(sendFile);
      	
      	while(config.state() && !config.finished())
      	{
      		safe_strcpy(tempHeader,config.nextHeader(),sizeof(tempHeader));

      		while(config.state() && !config.finished())
      		{
      			safe_strcpy(query,"insert into send (number,name,type) values ('",sizeof(query));
      			safe_strcpy(tempNumber,config.nextToken(),sizeof(tempNumber));
      			safe_strcpy(tempName,config.nextToken(),sizeof(tempName));
      			if(strcmp(tempNumber,"@") == 0)
      				break;
      			safe_strcat(query,tempNumber,sizeof(query));
      			safe_strcat(query,"','",sizeof(query));
      			safe_strcat(query,tempName,sizeof(query));
      			safe_strcat(query,"','",sizeof(query));
      			safe_strcat(query,tempHeader,sizeof(query));
      			safe_strcat(query,"');",sizeof(query));
      			printf("%s\n",query);
      			if (mysql_query(conn,query))
      			{
      				fprintf(stderr, "%s\n", mysql_error(conn));
      				goodConfig = false;
   			}
      		}
      	}
      	
}

int sms::sms_send(const char *message, const char *number)
{
	if(goodConfig == true)
	{
		char query[500] = "insert into outbox (number,text) values('";
		safe_strcat(query, number, sizeof(query));
		safe_strcat(query, "','", sizeof(query));
		safe_strcat(query, message, sizeof(query));
		safe_strcat(query, "');", sizeof(query));
	
		if (mysql_query(conn, query))
		{
      			fprintf(stderr, "%s\n", mysql_error(conn));
      			return -1;
      		}
 
		return 0;
	}
	else
	{
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
		return -1;
	}
}

bool sms::sms_send_member(const char *message, const char *name, const char *group)
{
	bool send = false;
	if(goodConfig == true)
	{

		char query[500] = "SELECT number FROM send WHERE name = '";
		safe_strcat(query,name,sizeof(query));
		safe_strcat(query,"' AND type = '",sizeof(query));
		safe_strcat(query,group,sizeof(query));
		safe_strcat(query,"');",sizeof(query));
		
		if (mysql_query(conn, query))
		{
      			fprintf(stderr, "%s\n", mysql_error(conn));
      			return send;
      		}
      		
      		res = mysql_use_result(conn);
      		
      		while ((row = mysql_fetch_row(res)) != NULL)
      		{
			
      		}
      		
	}
	else
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
	return send;
}

int main()
{

	sms mensajero("localhost", "root", "root", "gnokii","receive.conf","send.conf");
   
   	//mensajero.sms_send("Enviado con el programa","3315346974");

   /* send SQL query */
   //if (mysql_query(conn, "SELECT * FROM outbox")) {
      //fprintf(stderr, "%s\n", mysql_error(conn));
      //return 0;
   //}

   //res = mysql_use_result(conn);
   
   /* output fields 1 and 2 of each row */
   //while ((row = mysql_fetch_row(res)) != NULL)
      //printf("%s %s %s %s %s %s %s\n", row[1], row[2], row[3], row[4], row[5], row[6], row[7]);

   /* Release memory used to store results and close connection */
   //mysql_free_result(res);
   
}

