#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SERVER_PORT  5431
#define MAX_PENDING  5
#define MAX_LINE     256

int main(){
  struct sockaddr_in sin;
  char buf[MAX_LINE];
  int buf_len, addr_len;
  int s, new_s;

  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(SERVER_PORT);

  /* setup passive open */
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }
  if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
    perror("simplex-talk: bind");
    exit(1);
  }
  listen(s, MAX_PENDING);

 /* wait for connection, then receive and print text */
  while(1) {
    if ((new_s = accept(s, (struct sockaddr *)&sin, &addr_len)) < 0) {
      perror("simplex-talk: accept");
      exit(1);
    }
    memset(buf, 0, MAX_LINE);
    while (buf_len = recv(new_s, buf, sizeof(buf), 0)){
      fputs(buf, stdout);
      // when >>> Ciao-Ciao is entered, send an end message, and close both client socket
      // and server socket on the server side, exit with code 0.
      char checkbuf[14];
      strncpy(checkbuf, buf, 13); checkbuf[13] = '\0';
      if(strcmp(checkbuf, ">>> Ciao-Ciao") == 0){
        send(new_s, "Server closing...", 17, 0);
        close(new_s), close(s); exit(0);
      }
      // Capture the date and time of when the server receives the message.
      // In format year.month.date.time (time in hours:minutes)
      char date_buf[MAX_LINE]; // capture the date in a buffer
      memset(date_buf, 0, MAX_LINE);
      time_t t = time(NULL); // from time.h module
      struct tm currtime = *localtime(&t);
      snprintf(date_buf, MAX_LINE, "%d.%d.%d.%d:%d", 
      currtime.tm_year + 1900, currtime.tm_mon + 1, currtime.tm_mday, 
      currtime.tm_hour, currtime.tm_min);
      /* Send original line back to the client plus the
       date and time of when it was received on the server.*/
      char newbuf[MAX_LINE*2];
      memset(newbuf, 0, MAX_LINE*2);
      strcat(newbuf, buf); strcat(newbuf, date_buf);
      send(new_s, newbuf, sizeof(newbuf), 0);
    }
    close(new_s);
  }
}
