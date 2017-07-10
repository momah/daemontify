/**
    daemonwatch.h
    Purpose: Create server daemon proceses
    @author : momah (Mahrach Mahrach, Mohammed)
    @version : 1.0 10/07/2017
*/

#ifndef DAEMONWATCH_H
#define DAEMONWATCH_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>


// General
#define APP "daemONtify"
#define VERSION "0.1"

void *readBuffer(void *socket_desc);
void server(int argc, char *argv[]);
int daemonServer(int argc, char *argv[]);

// Other
//void showHelp();
//void showVersion();

#endif /* DAEMONWATCH_H */
