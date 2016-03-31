#ifndef INCLUDE_SURROUNDING_H_
#define INCLUDE_SURROUNDING_H_
#include "types.h"
#include <stdio.h>


void discover_surrounding();
void report_surrounding();


void _init_self_surrounding(struct self_surrounding** target, size_t count);
size_t _self_surrounding(struct self_surrounding** container, size_t max_count);
size_t get_self_surroundings(struct self_surrounding** container, size_t max);
#endif
