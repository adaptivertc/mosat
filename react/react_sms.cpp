/************************************************************************
 * This software is part of React, a control engine
 * Copyright (C) 2008 Donald Wayne Carr, Carlos René Diaz 
 *
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "db_point.h"
#include "db.h"
#include "sms.h"
#include "ap_config.h"

static sms_base *sms_object = NULL;

/***************************************************************/

const char * process_message(const char * message)
{
	return "i hope it works";
}

/***************************************************************/

void db_point_t::send_sms_group(const char *msg, const char *group)
{
  printf("Message for %s: %s\n", group, msg);
  if(sms_object == NULL)
  {
        printf("***********Error, SMS not enabled\n");
  	return;
  }
  else
  	sms_object->sms_send_group(msg,group);
  
}

/***************************************************************/

void react_t::init_sms(void)
{
	bool use_sms = ap_config.get_bool("SMS_ENABLED", false);

        if (!use_sms) 
	{
		printf("SMS is not Enabled - skipping initialization\n");
		return;
	}
	printf("Initializating SMS . . . \n");
        void *handle;

	handle = dlopen("libmysqlclient.so",RTLD_LAZY | RTLD_GLOBAL);
	if(handle== NULL)
	{
		fprintf(stderr,"%s\n",dlerror());
		exit(1);
	}

	handle = dlopen("../sms/librtsms.so",RTLD_LAZY);
	if(handle== NULL)
	{
		fprintf(stderr,"%s\n",dlerror());
		exit(1);
	}
	new_sms_t fn = (new_sms_t) dlsym(handle,"create_new_sms_object");
	
	char *error;
	
	if((error = dlerror()) != NULL)
	{
		fprintf(stderr,"%s\n",error);
		exit(1);
	}
	
	sms_object = (*fn)("./");
	printf("SMS initialization done.\n");

}

/***************************************************************/

void react_t::check_sms(void)
{
	if(sms_object == NULL)
		return;
	smsMessage message = sms_object->next_sms();
	if(!message.success())
		return;
	const char *reply = process_message(message.getMessage());
	sms_object->sms_send(reply,message.getNumber());
	
}

/***************************************************************/

