#include "self_start.h"
#include "log.h"
#include "config.h"
#include "command_socket.h"
#include "serviceEP.h"
#include "listening.h"
#include <string.h>
#include <pthread.h>
void discover_surrounding()
{
	size_t max_size = 10;
	void* surr = malloc(sizeof(struct self_surrounding));
	((struct self_surrounding*)surr)->dev_nearby =
			(struct dev_bluetooth*)malloc(sizeof(struct dev_bluetooth)*max_size);
	//self_surrounding* surr;
	//_init_self_surrounding(&surr,3);
	size_t avl = _self_surrounding((struct self_surrounding**)&surr, max_size);
	struct self_surrounding* ret = (struct self_surrounding*)surr;
	char filename[248] = WORKING_DIRECTORY;
	strcat(filename, "surrounding");
	FILE *f = fopen(filename, "w");
	if (f == NULL)
	{
		ERR_LOG("Error opening file!", __FILE__);
		return;
	}
	// lets start storing all shit
	fprintf(f, "%d\n", 100);
	fprintf(f, "%d\n", avl);
	for(int i = 0; i< avl; i++){
		struct dev_bluetooth device = *(ret->dev_nearby+i);
		fprintf(f, "%s %s %d\n", device.dev_id, device.dev_name, device.is_gateway);
	}
	fclose(f);
	free(surr);
	_write_message_to_socket(filename);
}
const char* _getfield(char* line, int num)
{
	const char* tok;
	for (tok = strtok(line, ";");
			tok && *tok;
			tok = strtok(NULL, ";\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}
void parse_surrounding(const char* filepath)
{
	FILE *f = fopen(filepath, "r");
	if (f == NULL)
	{
		ERR_LOG("Error opening file!", __FILE__);
		return;
	}
	char line[1024];
	size_t count = 0;
	size_t capacity = 10;
	struct self_surrounding* surrounding = malloc(sizeof(struct self_surrounding));
	surrounding->dev_nearby = malloc(capacity * sizeof(struct dev_bluetooth));
	surrounding->dev_nearby_count = 0;
	while (fgets(line, 1024, f))
	{
		char* tmp = strdup(line);
		//printf("Field 3 would be %s\n", _getfield(tmp, 3));
		// NOTE strtok clobbers tmp
		const char* id = _getfield(tmp, 1);
		const char* name = _getfield(tmp, 2);
		const char* gateway = _getfield(tmp, 3);
		struct dev_bluetooth device;
		strcpy(device.dev_id, id);
		strcpy(device.dev_name, name);
		device.is_gateway = gateway;
		surrounding->dev_nearby[count++] = device;
		if((count+1)> capacity)
		{
			capacity+=10;
			surrounding->dev_nearby =
					realloc(surrounding->dev_nearby, capacity*sizeof(struct dev_bluetooth));
		}
		free(tmp);
	}
	fclose(f);

}

void *thr_func(void *arg)
{
	char filepath[258] = WORKING_DIRECTORY;
	strcat(filepath, "command_socket");
	_create_command_socket(filepath);
	pthread_exit(NULL);
}
/// create a new thead. In the seperate thread start listening for new connection.
/// after creation of this thread, main thread should only be used for listening to inner port
/// and forwarding the message
void start_command_listening()
{
	int rc;
	pthread_t command_thread;
	if ((rc = pthread_create(&command_thread, NULL, thr_func, NULL))) {
		ERR_LOG("Could not start command listener", __FILE__);
		//fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
		return;
	}
}

void register_service_ep(const char* ep_path)
{
	char path[258] = WORKING_DIRECTORY;
	strcat(path, ep_path);
	_set_endpoint_subscription(ep_path);
}


void start_message_listening()
{
	_start_outer_incoming_port();
}
void exit_loop()
{
	FLUSH_ALL_LOG;
}

void start_loop()
{
	start_command_listening();
	start_message_listening();
}
