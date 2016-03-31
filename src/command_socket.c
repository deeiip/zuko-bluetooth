/*
 * command_socket.c
 *
 *  Created on: 12-Mar-2016
 *      Author: dipanjan
 */

#include "command_socket.h"
#include "log.h"
#include "self_start.h"
#include "forwarding.h"
#include "serviceEP.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close


bool valid_fname_format(const char* name)
{
	return true;
}
/// this functon should be thread safe
void parse_command_string(const char* arg)
{
	// svae_ptr is never freed. MLeak
	char* save_ptr = malloc(sizeof(char)*1024);
	char* token = strtok_r(arg, " ", &save_ptr);
	if(token==NULL)
	{
		// invalid command
		ERR_LOG("Invalid command passed", __FILE__);
		return;
	}
	else
	{
		if(strcmp(token,"send")==0)
		{
			token = strtok_r(NULL, " ", &save_ptr);
			char* pload = malloc(128*sizeof(char));
			strcpy(pload, token);
			token = strtok_r(NULL, " ", &save_ptr);
			char dest[20][19];
			size_t idx = 0;
			while(token!=NULL)
			{
				strcpy(dest[idx], token);
				idx++;
			}
			forward(pload, dest, idx);

		}
		else if(strcmp(token, "scan")==0)
		{
			discover_surrounding();
		}
		else{
			if(valid_fname_format(token))
			{
				_set_endpoint_subscription(token);
			}
		}
	}
}
void _create_command_socket(const char* filePath)
{
	int s, s2, len;
	socklen_t t;
	struct sockaddr_un local, remote;
	char command_str[100];

	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		//perror("socket");
		ERR_LOG("Can not create command socket", __FILE__);
		return;
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, filePath);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(s, (struct sockaddr *)&local, len) == -1) {
		//perror("bind");
		char message[128] = "Unsuccessful socket bind to ";
		strcat(message, local.sun_path);
		ERR_LOG(message, __FILE__);
		return;
	}

	if (listen(s, 5) == -1) {
		ERR_LOG("Can not start listening on socket", __FILE__);
		return;
	}

	for(;;) {
		int n;
		//printf("Waiting for a connection...\n");
		VER_LOG("Waiting for a command");
		t = sizeof(remote);
		if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
			perror("accept");
			ERR_LOG("Failed to accept a connection", __FILE__);
			return;
		}
		VER_LOG("Accepted command connection successfully");

		n = recv(s2, command_str, 100, 0);
		if (n < 0)
		{
			ERR_LOG("error receiving command", __FILE__);
			return;
		}
		command_str[n] = '\0';
		parse_command_string(command_str);

		close(s2);
	}
	return;
}
