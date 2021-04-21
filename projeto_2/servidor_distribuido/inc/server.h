#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 10109
#define MAX_BUFFER_SIZE 512

void create_server();
void stop_server();


#endif
