/*
 * log.c
 *
 *  Created on: 29-Nov-2015
 *      Author: dipanjan
 */
#include "log.h"
#include "types.h"
#include "config.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static log_buffer* err_log = NULL;
static log_buffer* verb_log = NULL;
static log_buffer* report_log = NULL;
void _init_log_buffer(log_buffer** log)
{
	if((*log)!=NULL)
	{
		return;
	}
	(*log) = malloc(sizeof(log_buffer));
	memset((*log)->char_buff, 0, sizeof((*log)->char_buff));
	(*log)->buff_length = 0;
}

void _flush_log_buffer(log_buffer* log, log_type typ)
{
	FILE* log_file;
	char file_name[100];
	memset(file_name, 0, sizeof(file_name));
	strcat(file_name, LOG_DIRECTORY);
	switch(typ)
	{
	case log_type_err:
		strcat(file_name, LOG_ERR_FILE_NAME);
		break;
	case log_type_verb:
		strcat(file_name, LOG_VERB_FILE_NAME);
		break;
	case log_type_rep:
		strcat(file_name, LOG_REP_FILE_NAME);
		break;
	}
	log_file = fopen(file_name, "a+");
	fprintf(log_file,"%s", log->char_buff);
	fclose(log_file);
	memset(log->char_buff, 0, sizeof(log->char_buff));
	log->buff_length = 0;

}

void _append_log_entry(log_buffer* log, log_type typ, const char* log_txt)
{
	size_t exp_len = log->buff_length + strlen(log_txt);
	if(exp_len>= sizeof(log->char_buff))
	{
		_flush_log_buffer(log, typ);
	}
	strcat(log->char_buff, "\n");
	strcat(log->char_buff, log_txt);
	log->buff_length = strlen(log->char_buff);
}
void _write_err(const char* message, const char* filename)
{
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//printf("Current local time and date: %s", asctime(timeinfo));
	//size_t time_len = strlen(asctime(timeinfo));
	if(err_log==NULL)
	{
		_init_log_buffer(&err_log);
	}
	char log_txt[1024];
	memset(log_txt, 0, sizeof(log_txt));
	strcat(log_txt, asctime(timeinfo));
	strcat(log_txt, filename);
	strcat(log_txt, ": ");
	strcat(log_txt, message);
	_append_log_entry(err_log, log_type_err, log_txt);
}
void _force_log_flush(log_buffer* log, log_type typ)
{
	_flush_log_buffer(log, typ);
}
void _write_log(const char* message)
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//printf("Current local time and date: %s", asctime(timeinfo));
	//size_t time_len = strlen(asctime(timeinfo));
	if (verb_log == NULL) {
		_init_log_buffer(&verb_log);
	}
	char log_txt[1024];
	memset(log_txt, 0, sizeof(log_txt));
	strcat(log_txt, asctime(timeinfo));
	strcat(log_txt, message);
	_append_log_entry(verb_log, log_type_verb, log_txt);
}
void _write_report(const char* message)
{
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//printf("Current local time and date: %s", asctime(timeinfo));
	//size_t time_len = strlen(asctime(timeinfo));
	if (report_log == NULL) {
		_init_log_buffer(&report_log);
	}
	char log_txt[1024];
	memset(log_txt, 0, sizeof(log_txt));
	strcat(log_txt, "\n");
	strcat(log_txt, asctime(timeinfo));
	strcat(log_txt, message);
	_append_log_entry(report_log, log_type_rep, log_txt);
}
void _force_flush_all()
{
	if (err_log != NULL) {
		_force_log_flush(err_log, log_type_err);
	}
	if (verb_log != NULL) {
		_force_log_flush(verb_log, log_type_verb);
	}
	if (report_log != NULL) {
		_force_log_flush(report_log, log_type_rep);
	}
}
