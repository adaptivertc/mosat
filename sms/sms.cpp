/*
The following example program (in file prog.c) should be compiled with this Unix command:

     gcc -I/usr/include/mysql sms.c -lmysqlclient -lnsl -lm -lz -o sms

*/

#include "sms.h"

void smsMessage::setData(const char *message, const char *number, const char *name, const char *group)
{
	safe_strcpy(this->message,message,sizeof(this->message));
	safe_strcpy(this->number,number,sizeof(this->number));
	safe_strcpy(this->name,name,sizeof(this->name));
	safe_strcpy(this->group,group,sizeof(this->group));
}

const char *smsMessage::getMessage()
{
	return message;
}

const char *smsMessage::getNumber()
{
	return number;
}

const char *smsMessage::getName()
{
	return name;
}

const char *smsMessage::getGroup()
{
	return group;
}

void smsMessage::setSuccess(bool x)
{
	status = x;
}

bool smsMessage::success()
{
	return status;
}

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
	{	while(!feof(fp) && temp != '[')
		{

			temp = fgetc(fp);
			
			if(temp == '|')
			{
				token[i] = '\0';
				return token;
			}
				
			if(temp != '\n')
			{
				token[i] = temp;
				i++;
			}
		}
		
		if(temp == '[')
			return "@";
		
		if(feof(fp))
		{
			goodConfig = false;
			finish = true;
			return "@";
		}
	
		
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
			count = (short int)ceil((double)strlen(multiMsg)/155);
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


sms::sms(const char *host, const char *user, const char *password, const char *database, const char *smsdConfig, const char *receiveFile, const char *sendFile)
{

	char query[300];
	char tempHeader[NAMEMAXLEN];
	char tempNumber[NUMMAXLEN];
	char tempName[NAMEMAXLEN];
	goodConfig = true;
	sprintf(gammuFile,"%s",smsdConfig);
	
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

      			if (mysql_query(conn,query))
      			{
      				fprintf(stderr, "%s\n", mysql_error(conn));
      				goodConfig = false;
   			}
      		}
      	}
      	
}

/*bool sms::sms_send(const char *message, const char *number)
{
	if(goodConfig == true)
	{
		char query[500];
		
		printf("%s",message);
		
		if(strlen(message) <= 160)
		{
			sprintf(query,"INSERT INTO outbox (number,text) VALUES ('%s','%s');",number,message);
			if (mysql_query(conn, query))
			{
      				fprintf(stderr, "%s\n", mysql_error(conn));
      				return false;
      			}
		}
		else
		{
			smsSplit spliter(message);
		
			for(int i = 1; i <= spliter.getCount(); i++)
			{
				sprintf(query,"INSERT INTO outbox (number,text) VALUES ('%s','(%d/%d)%s');",number,i,spliter.getCount(),spliter.getNextMsg());
				printf("%s",query);
				if(mysql_query(conn,query))
				{
					fprintf(stderr,"%s\n",mysql_error(conn));
					return false;
				}
			}
		}
 
		return true;
	}
	else
	{
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
		return false;
	}
}*/

bool sms::sms_send(const char *message, const char *number)
{
	if(goodConfig == true)
	{
		char query[500];
		
		printf("%s",message);
		
		if(strlen(message) <= 160)
		{
			//system("ls");
			sprintf(query,"echo \"%s\" | gammu --sendsmsdsms TEXT %s MYSQL %s",message,number,gammuFile);
			if (system(query) != 0)
			{
      				fprintf(stderr,"Error al enviar mensaje\n");
      				return false;
      			}
		}
		else
		{
			smsSplit spliter(message);
		
			for(int i = 1; i <= spliter.getCount(); i++)
			{
				sprintf(query,"echo \"(%d/%d)%s\" | gammu --sendsmsdsms TEXT %s MYSQL %s",i,spliter.getCount(),spliter.getNextMsg(),number,gammuFile);
				printf("%s",query);
				if(system(query) != 0)
				{
					fprintf(stderr,"Error al enviar el mensaje\n");
					return false;
				}
			}
		}
 
		return true;
	}
	else
	{
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
		return false;
	}
}

bool sms::sms_send_member(const char *message, const char *name, const char *group)
{
	MYSQL_RES *res;
	bool send = false;
	
	if(goodConfig == true)
	{

		char query[500] = "SELECT number FROM send WHERE name = '";
		safe_strcat(query,name,sizeof(query));
		safe_strcat(query,"' AND type = '",sizeof(query));
		safe_strcat(query,group,sizeof(query));
		safe_strcat(query,"';",sizeof(query));
		
		
		if (mysql_query(conn, query))
		{
      			fprintf(stderr, "%s\n", mysql_error(conn));
      			return send;
      		}
      		
      		res = mysql_store_result(conn);
      		
      		
      		while ((row = mysql_fetch_row(res)) != NULL)
      		{
      			send = sms_send(message,row[0]);
      			
      		}
      		
      		mysql_free_result(res);
      		
      		return send;
	}
	else
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
	return send;
}

/*bool sms::sms_send_member(const char *message, const char *name, const char *group)
{
	bool send = false;
	if(goodConfig == true)
	{

		char query[500] = "SELECT number FROM send WHERE name = '";
		safe_strcat(query,name,sizeof(query));
		safe_strcat(query,"' AND type = '",sizeof(query));
		safe_strcat(query,group,sizeof(query));
		safe_strcat(query,"';",sizeof(query));
		
		
		if (mysql_query(conn, query))
		{
      			fprintf(stderr, "%s\n", mysql_error(conn));
      			return send;
      		}
      		
      		res = mysql_store_result(conn);
      		
      		
      		while ((row = mysql_fetch_row(res)) != NULL)
      		{
      			if(strlen(message) <= 160)
      			{
      				sprintf(query,"INSERT INTO outbox (number,text) VALUES ('%s','%s');",row[0],message);
				if(mysql_query(conn,query))
				{
					fprintf(stderr,"%s\n",mysql_error(conn));
				}
				send = true;
      			}
      			else
      			{
      				smsSplit spliter(message);
				for(int i = 1; i <= spliter.getCount(); i++)
				{
					sprintf(query,"INSERT INTO outbox (number,text) VALUES ('%s','(%d/%d)%s');",row[0],i,spliter.getCount(),spliter.getNextMsg());
					if(mysql_query(conn,query))
					{
						fprintf(stderr,"%s\n",mysql_error(conn));
					}
					send = true;
				}
			}
      		}
      		
      		return send;
	}
	else
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
	return send;
}*/

bool sms::sms_send_group(const char *message, const char *group)
{
	MYSQL_RES *res;
	bool send = false;
	if(goodConfig == true)
	{

		char query[500] = "SELECT number FROM send WHERE type = '";
		safe_strcat(query,group,sizeof(query));
		safe_strcat(query,"';",sizeof(query));
		
		
		if (mysql_query(conn, query))
		{
      			fprintf(stderr, "%s\n", mysql_error(conn));
      			return send;
      		}
      		
      		res = mysql_store_result(conn);
      		
      		
      		while ((row = mysql_fetch_row(res)) != NULL)
      		{
      			send = sms_send(message,row[0]);
      		}
      		
      		mysql_free_result(res);
      		
      		return send;
	}
	else
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
	return send;
}

/*bool sms::sms_send_group(const char *message, const char *group)
{
	bool send = false;
	if(goodConfig == true)
	{

		char query[500] = "SELECT number FROM send WHERE type = '";
		safe_strcat(query,group,sizeof(query));
		safe_strcat(query,"';",sizeof(query));
		
		
		if (mysql_query(conn, query))
		{
      			fprintf(stderr, "%s\n", mysql_error(conn));
      			return send;
      		}
      		
      		res = mysql_store_result(conn);
      		
      		
      		while ((row = mysql_fetch_row(res)) != NULL)
      		{
      			if(strlen(message) <= 160)
      			{
      				sprintf(query,"INSERT INTO outbox (number,text) VALUES ('%s','%s');",row[0],message);
				if(mysql_query(conn,query))
				{
					fprintf(stderr,"%s\n",mysql_error(conn));
				}
				send = true;
      			}
      			else
      			{
      				smsSplit spliter(message);
				for(int i = 1; i <= spliter.getCount(); i++)
				{
					sprintf(query,"INSERT INTO outbox (number,text) VALUES ('%s','(%d/%d)%s');",row[0],i,spliter.getCount(),spliter.getNextMsg());
					if(mysql_query(conn,query))
					{
						fprintf(stderr,"%s\n",mysql_error(conn));
					}
					send = true;
				}
			}
      		}
      		
      		return send;
	}
	else
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
	return send;
}*/

bool sms::sms_send_all(const char *message)
{
	MYSQL_RES *res;
	bool send = false;
	if(goodConfig == true)
	{

		char query[500] = "SELECT number FROM send;";
		
		if (mysql_query(conn, query))
		{
      			fprintf(stderr, "%s\n", mysql_error(conn));
      			return send;
      		}
      		
      		res = mysql_store_result(conn);
      		
      		
      		while ((row = mysql_fetch_row(res)) != NULL)
      		{
      			send = sms_send(message,row[0]);
      		}
      		
      		mysql_free_result(res);
      		
      		return send;
	}
	else
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
	return send;
}

/*bool sms::sms_send_all(const char *message)
{
	bool send = false;
	if(goodConfig == true)
	{

		char query[500] = "SELECT number FROM send;";
		
		if (mysql_query(conn, query))
		{
      			fprintf(stderr, "%s\n", mysql_error(conn));
      			return send;
      		}
      		
      		res = mysql_store_result(conn);
      		
      		
      		while ((row = mysql_fetch_row(res)) != NULL)
      		{
      			if(strlen(message) <= 160)
      			{
      				sprintf(query,"INSERT INTO outbox (number,text) VALUES ('%s','%s');",row[0],message);
				if(mysql_query(conn,query))
				{
					fprintf(stderr,"%s\n",mysql_error(conn));
				}
				send = true;
      			}
      			else
      			{
      				smsSplit spliter(message);
				for(int i = 1; i <= spliter.getCount(); i++)
				{
					sprintf(query,"INSERT INTO outbox (number,text) VALUES ('%s','(%d/%d)%s');",row[0],i,spliter.getCount(),spliter.getNextMsg());
					if(mysql_query(conn,query))
					{
						fprintf(stderr,"%s\n",mysql_error(conn));
					}
					send = true;
				}
			}
      		}
      		
      		return send;
	}
	else
		printf("No se puede enviar mensaje, no se configuro apropiadamente la lista de destinatarios");
	return send;
}*/

smsMessage sms::next_sms()
{
	MYSQL_RES *res;
	const char *query = "SELECT TextDecoded, SenderNumber FROM inbox WHERE Processed = 'false';";
	
	if(mysql_query(conn,query))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		actualMessage.setSuccess(false);
	}
	
	res = mysql_store_result(conn);
	
	if((row = mysql_fetch_row(res)) != NULL)
	{
		actualMessage.setData(row[0],row[1],"","");
		actualMessage.setSuccess(true);
	}
	
	mysql_free_result(res);
	
	return actualMessage;
}

smsMessage sms::next_sms_number(const char *number)
{
	MYSQL_RES *res;
	char query[100];
	
	sprintf(query,"SELECT TextDecoded, SenderNumber FROM inbox WHERE SenderNumber = '%s';",number);
	
	if(mysql_query(conn,query))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		actualMessage.setSuccess(false);
	}
	
	res = mysql_store_result(conn);
	
	if((row = mysql_fetch_row(res)) != NULL)
	{
		actualMessage.setData(row[0],row[1],"","");
		actualMessage.setSuccess(true);
	}
	
	mysql_free_result(res);
	
	return actualMessage;
}

smsMessage sms::next_sms_group(const char *group)
{
	MYSQL_RES *res;
	char query[250];
	
	sprintf(query,"SELECT inbox.TextDecoded, receive.number, receive.name, receive.type FROM inbox, receive WHERE inbox.Processed = 'false' AND inbox.SenderNumber = receive.number AND receive.type = '%s';",group);
	
	if(mysql_query(conn,query))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		actualMessage.setSuccess(false);
	}
	
	res = mysql_store_result(conn);
	
	if((row = mysql_fetch_row(res)) != NULL)
	{
		actualMessage.setData(row[0],row[1],row[2],row[3]);
		actualMessage.setSuccess(true);
	}
	
	mysql_free_result(res);
	
	return actualMessage;
}

void sms::sms_prueba()
{
	MYSQL_RES *res;
	char query[1000];
	
	while(true)
	{
	

	//SELECT SenderNumber, TextDecoded, ID FROM inbox WHERE processed = 'false' ORDER BY ReceivingDateTime
	sprintf(query,"SELECT SenderNumber, TextDecoded, ID FROM inbox WHERE processed = 'false' ORDER BY ReceivingDateTime;");
	if(mysql_query(conn,query))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
	}
	
	res = mysql_store_result(conn);
	
	printf("%s\n",query);
	
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s\n",row[0]);
		sprintf(query,"Se recibio el siguiente mensaje: %s",row[1]);
		printf("%s\n",query);
		printf("Antes del send");
		sms_send(query,row[0]);
		sprintf(query,"UPDATE inbox SET processed = 'true' WHERE ID = '%s';",row[2]);
		printf("%s\n",query);
		if(mysql_query(conn,query))
		{
			fprintf(stderr,"%s\n",mysql_error(conn));
		}
	}
		mysql_free_result(res);
		sleep(10);
	}
	
}

int main(int argc, char *arvg[])
{
	
	sms mensajero("localhost", "root", "root", "gammu","gammusmsdrc.conf","receive.conf","send.conf");
	
	smsMessage mensaje;
	
	mensaje = mensajero.next_sms();
	
	printf("Mensaje: %s\nNumero: %s\nNombre: %s\nGrupo: %s\n",mensaje.getMessage(),mensaje.getNumber(),mensaje.getName(),mensaje.getGroup());
	//mensajero.sms_send("LOL","3311709069");
	//mensajero.sms_prueba();
   	//mensajero.sms_send_member("Calando","Rene","Normal");
   	//mensajero.sms_send_member("Lalalalala","Toño","Normal");
   	//mensajero.sms_send_all("Testing");
   	
   	//bool paso = mensajero.sms_send_group("Segunda prueba, por favor contesta el mensaje","Normal");

	//if(paso)
	//	printf("Funciona");
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

