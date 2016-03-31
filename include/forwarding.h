#ifndef FORWARDING_H_
#define FORWARDING_H_

#include <stdint.h>



//int forward(payload);
// send the data and specify a list of addresses to send to
void forward(const char*, const char*, uint8_t);
#endif
