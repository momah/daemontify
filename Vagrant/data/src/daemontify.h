/**
    daemontify.h
    Purpose: Client daemon, Watches directories using inotify,
    sending statistics using client-server model.
    @author Momah (Mahrach Mahrach, Mohammed)
    @version 1.0 10/07/2017
*/

#ifndef DAEMONTIFY_H
#define DAEMONTIFY_H

// Inotify
#include "daemonwatch.h"
#include <sys/inotify.h>
#include <limits.h>
#include <netdb.h>

// Utilities
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))


void showEvent(struct inotify_event *e, int sockfd);
int inotify(int argc, char *argv[]);
int daemonInotify(int argc, char *argv[]);


#endif /* DAEMONTIFY_H */
