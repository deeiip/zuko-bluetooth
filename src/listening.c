/*
 * listening.c
 *
 *  Created on: 30-Nov-2015
 *      Author: dipanjan
 */
#include "listening.h"
#include "config.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

// name should be created from the WORKING DIRECTORY macro
FILE* _open_file(const char* filename)
{
	FILE *write_ptr;
	write_ptr = fopen(filename, "wb");
	return write_ptr;
}
void _write_to_file(FILE* fptr, const char* buffer)
{
	fwrite(buffer, sizeof(buffer), 1, fptr);
}
void _close_file(FILE* fptr)
{
	fclose(fptr);
}
void _start_outer_incoming_port() {
	struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };

	int s;
	socklen_t opt = sizeof(rem_addr);
	REPORT_LOG("Creating socket");
	// allocate socket
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	// bind socket to port 1 of the first available
	// local bluetooth adapter
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = *BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) OUTER_LISTEN_CHANNEL;
	bind(s, (struct sockaddr *) &loc_addr, sizeof(loc_addr));
	char buff[256] = "Bind successful to ";
	char port[3];
	sprintf(port, "%d", port);
	//itoa(OUTER_LISTEN_CHANNEL, port, 10);
	strcpy(buff, port);
	REPORT_LOG(buff);
	// put socket into listening mode
	listen(s, 1);

	while (1) {
		int con;
		char message_file_path[256];
		// accept one connection
		int client;
		client = accept(s, (struct sockaddr *) &rem_addr, &opt);
		char buf[1024] = { 0 };
		ba2str(&rem_addr.rc_bdaddr, buf);
		//fprintf(stderr, "accepted connection from %s\n", buf);
		char message[256] = "accepted connection from ";
		strcat(message, buf);
		REPORT_LOG(message);
		memset(buf, 0, sizeof(buf));
		// read data from the client
		int init_idx = 0;
		FILE* file = _open_file(message_file_path);
		while(recv(client, buf, 1024, init_idx)>0)
		{
			init_idx+=1024;
			_write_to_file(file, buf);
			VER_LOG(buf);
		}
		_close_file(file);
		_write_message_to_socket(message_file_path);
		//int bytes_read;
		//bytes_read = read(client, buf, sizeof(buf));
		//if (bytes_read > 0) {
		//	printf("received [%s]\n", buf);
		//}

		// close connection
		close(client);
	}
	close(s);
	return;
}


void _process_incoming(char* raw_data)
{

}

void _start_self_endpoint()
{

}
