#ifndef INCLUDE_LISTENING_H_
#define INCLUDE_LISTENING_H_



void attach_listener();
void start_listening();
void stop_listening();

// incoming port should be listening for connection constantly
void _start_outer_incoming_port();

// self endpoint should be
void _start_self_endpoint();

#endif
