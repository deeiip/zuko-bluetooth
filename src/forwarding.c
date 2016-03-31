/*
 * forwarding.c
 *
 *  Created on: 05-Mar-2016
 *      Author: dipanjan
 */
#include "config.h"
#include "forwarding.h"
#include "log.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <malloc.h>


long _get_file_buffer(const char* filename, char** _buffer)
{
	REPORT_LOG("starting file read");
	FILE *fileptr;
	char *buffer = *_buffer;
	long filelen;

	fileptr = fopen(filename, "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (char *)malloc((filelen)*sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, fileptr); // Read in the entire file
	REPORT_LOG("File read successful");
	fclose(fileptr); // Close the file
	return filelen;
}

void _forward_unit(const char* data, const char* _dest)
{
	struct sockaddr_rc addr = { 0 };
	int s, status;
	char dest[18];
	strcpy(dest, _dest);

	// allocate a socket
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	// set the connection parameters (who to connect to)
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) OUTER_LISTEN_CHANNEL ;
	str2ba( dest, &addr.rc_bdaddr );

	// connect to server
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
	// send a message
	if( status == 0 ) {
		char* buffer = NULL;
		long f_len = _get_file_buffer(data, &buffer);
		status = write(s, buffer, f_len);
	}

	if( status < 0 )
	{
		//perror("uh oh");
		char error_message[128] = "Could not connect. Error sending data to ";
		strcat(error_message, dest);
		ERR_LOG(error_message, __FILE__);
	}

	close(s);
}

void forward(const char* data, const char* targets, uint8_t count)
{
	for(uint8_t i = 0; i < count; i++){
		_forward_unit(&data, (targets+(i*19)));
	}

}



