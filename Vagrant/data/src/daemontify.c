/**
    daemontify.c
    Purpose: Daemon with inotify and sending statistics using client-server model.
    @author Momah (Mahrach Mahrach, Mohammed)
    @version 1.0 10/07/2017
*/

#include "daemontify.h"

// Signal handling

void showEvent(struct inotify_event *e, int sockfd) {
  char *buffer = "";
  switch(e->mask & IN_ALL_EVENTS){
    case IN_ACCESS:
      // syslog(LOG_NOTICE, "IN_ACCESS");
      buffer = "IN_ACCESS";
      break;

	  case IN_ATTRIB:
      // syslog(LOG_NOTICE, "IN_ATTRIB");
      buffer = "IN_ATTRIB";
      break;

    case IN_CLOSE_WRITE:
      // syslog(LOG_NOTICE, "IN_CLOSE_WRITE");
      buffer = "IN_CLOSE_WRITE";
      break;

    case IN_CLOSE_NOWRITE:
      // syslog(LOG_NOTICE, "IN_CLOSE_NOWRITE");
      buffer = "IN_CLOSE_NOWRITE";
      break;

    case IN_CREATE:
      // syslog(LOG_NOTICE, "IN_CREATE");
      buffer = "IN_CREATE";
      break;

    case IN_DELETE:
      // syslog(LOG_NOTICE, "IN_DELETE");
      buffer = "IN_DELETE";
      break;

    case IN_DELETE_SELF:
      // syslog(LOG_NOTICE, "IN_DELETE_SELF");
      buffer = "IN_DELETE_SELF";
      break;

    case IN_MODIFY:
      // syslog(LOG_NOTICE, "IN_MODIFY");
      buffer = "IN_MODIFY";
      break;

    case IN_MOVE_SELF:
      // syslog(LOG_NOTICE, "IN_MOVE_SELF");
      buffer = "IN_MOVE_SELF";
      break;

    case IN_MOVED_FROM:
      // syslog(LOG_NOTICE, "IN_MOVE_FROM");
      buffer = "IN_MOVE_FROM";
      break;

    case IN_MOVED_TO:
      // syslog(LOG_NOTICE, "IN_MOVE_TO");
      buffer = "IN_MOVE_TO";
      break;

    case IN_OPEN:
      // syslog(LOG_NOTICE, "IN_OPEN");
      buffer = "IN_OPEN";
      break;

    case IN_ALL_EVENTS:
      // syslog(LOG_NOTICE, "IN_ALL_EVENTS");
      buffer = "IN_ALL_EVENTS";
      break;

    case IN_MOVE:
      // syslog(LOG_NOTICE, "IN_MOVE");
      buffer = "IN_MOVE";
      break;

    case IN_CLOSE:
      // syslog(LOG_NOTICE, "IN_CLOSE");
      buffer = "IN_CLOSE";
      break;

    case IN_DONT_FOLLOW:
      // syslog(LOG_NOTICE, "IN_DONT_FOLLOW");
      buffer = "IN_DONT_FOLLOW";
      break;

    case IN_MASK_ADD:
      // syslog(LOG_NOTICE, "IN_MASK_ADD");
      buffer = "IN_MASK_ADD";
      break;

    case IN_ONESHOT:
      // syslog(LOG_NOTICE, "IN_ONESHOT");
      buffer = "IN_ONESHOT";
      break;

    case IN_ONLYDIR:
      // syslog(LOG_NOTICE, "IN_ONLYDIR");
      buffer = "IN_ONLYDIR";
      break;

    case IN_IGNORED:
      // syslog(LOG_NOTICE, "IN_IGNORED");
      buffer = "IN_IGNORED";
      break;

    case IN_ISDIR:
      // syslog(LOG_NOTICE, "IN_ISDR");
      buffer = "IN_ISDR";
      break;

    case IN_Q_OVERFLOW:
      // syslog(LOG_NOTICE, "IN_Q_OVERFLOW");
      buffer = "IN_Q_OVERFLOW";
      break;

    case IN_UNMOUNT:
      // syslog(LOG_NOTICE, "IN_UNMOUNT");
      buffer = "IN_UNMOUNT";
      break;
  }

  write(sockfd, buffer, strlen(buffer));
}

int inotify(int argc, char *argv[]){
  int fd, wd, i;
  char buf[BUF_LEN];
  size_t num;
  char *e;
  struct inotify_event *event;

  fd = inotify_init();

  // Error
  if (fd == -1) {
    syslog(LOG_NOTICE, "Error: fd");
    return 0;
  }

  wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
  // Error
  if (wd == -1) {
    syslog(LOG_NOTICE, "Error: wd");
    return 0;
  }

  // Create the client
  int sockfd, portnumber;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  // Add the portnumber
  portnumber = atoi(argv[3]);

  // Create the socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // Get the server
  server = gethostbyname(argv[2]);

  // Error controller
  bzero((char*)&serv_addr, sizeof(serv_addr));

  // Server information
  serv_addr.sin_family = AF_INET;
  bcopy((char*)server->h_addr,
        (char*)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portnumber);

  // Connection
  connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  while (1) {
    num = read(fd, buf, BUF_LEN);
    // Error
    if (num == -1) {
      syslog(LOG_NOTICE, "Error: num");
      return 0;
    }

    for (e = buf; e < buf + num;) {
      event = (struct inotify_event *)e;
      showEvent(event, sockfd);
      e += sizeof(struct inotify_event) + event->len;
    }
  }

  // Close the socket connection
  close(sockfd);
}

int daemonInotify(int argc, char *argv[]) {
   /* Our process ID and Session ID */
   pid_t pid, sid;

   /* Fork off the parent process */
   pid = fork();
   if (pid < 0) {
      exit(EXIT_FAILURE);
   }

   /* If we got a good PID, then
      we can exit the parent process. */
   if (pid > 0) {
      exit(EXIT_SUCCESS);
   }

   /* Change the file mode mask */
   umask(0);

   /* Open any logs here */

   /* Create a new SID for the child process */
   sid = setsid();

   if (sid < 0) {
      /* Log the failure */
      exit(EXIT_FAILURE);
   }

   /* Change the current working directory */
   if ((chdir("/")) < 0) {
      /* Log the failure */
      exit(EXIT_FAILURE);
   }

   /* Close out the standard file descriptors */
   close(STDIN_FILENO);
   close(STDOUT_FILENO);
   close(STDERR_FILENO);

   /* Daemon-specific initialization goes here */

   /* Funcionality */
   inotify(argc, argv);

   exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
   daemonInotify(argc, argv);
}
