/*
 * serviceEP.h
 *
 *  Created on: 30-Nov-2015
 *      Author: dipanjan
 */

#ifndef INCLUDE_SERVICEEP_H_
#define INCLUDE_SERVICEEP_H_
#include "types.h"

void _set_endpoint_subscription( char*);
void _destroy_current_endpoint_subscription();
void _write_message_to_socket(char*);
#endif /* INCLUDE_SERVICEEP_H_ */
