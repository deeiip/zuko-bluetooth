#include "serviceEP.h"
#include "log.h"
#include "config.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


static char* current_subscriber;
static int subscriber_socket;
//void _init_current_endpoint_subscription(current_endpoint_subscription** arg)
//{
//	(*arg) = malloc(sizeof(current_endpoint_subscription));
//	(*arg)->arr = malloc(sizeof(service_endpoint*));
//	(*arg)->count = 0;
//}

void _set_endpoint_subscription( char* path)
{
	/*size_t current_idx = (*arg)->count;
	(*arg)->arr[current_idx]->ep_type = ep.ep_type;
	(*arg)->arr[current_idx]->ep_file_path = malloc(sizeof(char)* (strlen(ep.ep_file_path)+1));
	strcpy((*arg)->arr[current_idx]->ep_file_path, ep.ep_file_path);
	(*arg)->count++;
	current_idx++;
	(*arg)->arr = realloc((current_idx+1)*sizeof(service_endpoint));*/
	//	size_t current_idx = (*arg)->count;
	//	(*arg)->arr[current_idx] = ep;
	//	(*arg)->count++;
	//	current_idx++;
	//	(*arg)->arr = realloc((*arg)->arr, (current_idx+1)*sizeof(service_endpoint*));
	size_t req_size = strlen(path);
	req_size+=1;
	current_subscriber = malloc(req_size * sizeof(char));
	strcat(current_subscriber, path);
	//subscriber_socket = _create_subscription_socket(current_subscriber);
}

//void _create_subscription_ep(const char* ep_path, message_type m_type)
//{
//	service_endpoint temp;
//	temp.ep_file_path = malloc(strlen(ep_path+1)*sizeof(char));
//	strcpy(temp.ep_file_path, ep_path);
//	temp.ep_type = m_type;
//	if(current_subscribers==NULL)
//	{
//		_init_current_endpoint_subscription(&current_subscribers);
//	}
//	_add_endpoint_subscription(&current_subscribers, &temp);
//}

void _destroy_current_endpoint_subscription()
{
	// destroy all individual endpoint
	//	for(size_t i = 0; i < (*arg)->count; i++)
	//	{
	//		service_endpoint* target = (*arg)->arr[i];
	//		free(target);
	//	}
	//	// destroy subscription itself
	//	free(*arg);
	close(subscriber_socket);
	current_subscriber = NULL;
}

// it'd create a file socket for the upper level
// for communication. This function assumes server socket
// is listening to the socket_path

//void _write_file_to_socket(int sockt, const char* filename)
//{
//	int file_d = open(filename, O_RDONLY);
//	struct stat f_stat;
//	fstat(file_d, &f_stat);
//	void* addr = mmap(NULL, f_stat.st_size, PROT_READ, MAP_SHARED, file_d, 0);
//	int status = write(sockt, addr, f_stat.st_size);
//	if(status < 0)
//	{
//		ERR_LOG("Unsuccessful file write to socket", __FILE__);
//	}
//	else
//	{
//		REPORT_LOG("Socket file write successful");
//	}
//}
void _write_message_to_socket(char* msg)
{
	REPORT_LOG("Start processing message");
	//	if(msg.typ == message_type_data)
	//	{
	//		char buff[256] = "Requesting data message at ";
	//		strcat(buff, msg.data);
	//		REPORT_LOG(buff);
	//		_write_file_to_socket(sockt, msg.data);
	//		return;
	//	}
//	int s, len;
//	struct sockaddr_un remote;
//	//char str[100];
//
//	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
//		//perror("socket");
//		ERR_LOG("Socket creation failed", __FILE__);
//		return;
//	}
//
//	//printf("");
//	REPORT_LOG("Trying to connect...");
//	remote.sun_family = AF_UNIX;
//	strcpy(remote.sun_path, current_subscriber);
//	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
//	if (connect(s, (struct sockaddr *) &remote, len) == -1) {
//		//perror("connect");
//		char buff[128] = {0};
//		strcat(buff, "Could not connect to socket");
//		strcat(buff, remote.sun_path);
//		ERR_LOG(buff, __FILE__);
//		return;
//	}
//
//	//printf("Connected.\n");
//	char buff[128] = {0};
//	strcat(buff, "Connected to ");
//	strcat(buff, remote.sun_path);
//	REPORT_LOG(buff);
//	int status = send(subscriber_socket, msg, strlen(msg), 0 );
//	if(status < 0 )
//	{
//		ERR_LOG("Unsuccessful message write to socket", __FILE__);
//	}
//	else
//	{
//		REPORT_LOG("Socket message write successful");
//	}
	int s, t, len;
	    struct sockaddr_un remote;
	    char str[100];

	    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
	        perror("socket");
	        exit(1);
	    }

	    printf("Trying to connect...\n");

	    remote.sun_family = AF_UNIX;
	    strcpy(remote.sun_path, current_subscriber);
	    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
	        perror("connect");
	        exit(1);
	    }

	    printf("Connected.\n");
	    send(s, msg, strlen(msg), 0);
//	    while(printf("> "), fgets(str, 100, stdin), !feof(stdin)) {
//	        if (send(s, str, strlen(str), 0) == -1) {
//	            perror("send");
//	            exit(1);
//	        }
//
//	        if ((t=recv(s, str, 100, 0)) > 0) {
//	            str[t] = '\0';
//	            printf("echo> %s", str);
//	        } else {
//	            if (t < 0) perror("recv");
//	            else printf("Server closed connection\n");
//	            exit(1);
//	        }
//	    }

	    close(s);

}


