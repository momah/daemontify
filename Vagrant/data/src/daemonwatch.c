/**
    daemonwatch.c
    Purpose: Server Daemon with inotify and sending statistics using client-server model.
    @author momah (Mahrach Mahrach, Mohammed)
    @version 1.0 10/07/2017
*/


#include "daemonwatch.h"

void *readBuffer(void *socket_desc) {
  int i;
  char buffer[256];

  while(1){
    bzero(buffer, 256);
    // Read
    i = read(*(int *)socket_desc, buffer, 255);

    if(i > 0) {
      syslog(LOG_NOTICE, "%s", buffer);
    } else {
      if(i == 0){
        break;
      }
    }
  }
}

void server(int argc, char *argv[]) {

  int sockfd, newsockfd, portnumber;
  socklen_t clilongitud;
  struct sockaddr_in serv_addr, cli_addr;

  // Create the socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char*)&serv_addr, sizeof(serv_addr));

  // Add the port number
  portnumber = atoi(argv[1]);

  // Server information
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portnumber);

  bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  // Listen in the port, maximium five connections
  listen(sockfd, 5);

  clilongitud = sizeof(cli_addr);

  // Accept the connections

  while(newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilongitud)) {

    pthread_t sniffer_thread;

    if(pthread_create( &sniffer_thread , NULL,  readBuffer, (void*) &newsockfd) < 0) {
      syslog(LOG_NOTICE, "Error: Can't create a thread");
    }
  }

  if(newsockfd < 0){
    syslog(LOG_NOTICE, "Error socket");
  }

  // Close the connection
  close(newsockfd);
  close(sockfd);
}

int daemonServer(int argc, char *argv[]) {
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

   // Change the file mode mask for permissions
   umask(0);

   /* Open any logs here */
   syslog(LOG_INFO, "Starting...");

   /* Create a new SID for the child process */
   sid = setsid();

   if (sid < 0) {
      /* Log the failure */
      syslog(LOG_ERR, "Failed to start a new sesion");
      exit(EXIT_FAILURE);
   }

   /* Change the current working directory */
   if ((chdir("/")) < 0) {
      /* Log the failure */
      syslog(LOG_ERR, "Failed to change directory");
      exit(EXIT_FAILURE);
   }

   /* Close out the standard file descriptors */
   close(STDIN_FILENO);
   close(STDOUT_FILENO);
   close(STDERR_FILENO);

   /* Daemon-specific initialization goes here */
   syslog(LOG_NOTICE, "Started %s", APP);
   /* Funcionality */
   server(argc, argv);

   exit(EXIT_SUCCESS);

   // Close logs
   syslog(LOG_NOTICE, "Finished %s", APP);
   closelog();
   return 0;
}

int main(int argc, char *argv[]) {
  daemonServer(argc, argv);
}
