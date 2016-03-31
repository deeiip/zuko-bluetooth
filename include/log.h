/*
 * log.h
 *
 *  Created on: 29-Nov-2015
 *      Author: dipanjan
 */

#ifndef INCLUDE_LOG_H_
#define INCLUDE_LOG_H_

#define ERR_LOG(message, filename) _write_err(message, filename);
#define VER_LOG(message) _write_log(message);
#define REPORT_LOG(message) _write_report(message);
#define FLUSH_ALL_LOG _force_flush_all();


void _write_err(const char*, const char*);
void _write_log(const char*);
void _write_report(const char*);
void _force_flush_all();

#endif /* INCLUDE_LOG_H_ */
