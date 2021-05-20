#ifndef HEADER_H // フラグが立っていなければ（ if HEADER_H is not defined ）
#define HEADER_H

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_HTDOCS "../htdocs"

// void receive_request(char *http_request, int sock);

// void generate_response(char *http_request, char *path_name);
// void divide_space(char *http_status[], char *buf);
// int path_status(char *path_name);

// void send_response(char *path_name, int sock);

#endif
