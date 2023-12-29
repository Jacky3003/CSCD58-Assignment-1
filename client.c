#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT 5431
#define MAX_LINE 256

int
main(int argc, char * argv[])
{
  FILE *fp;
  struct hostent *hp;
  struct sockaddr_in sin;
  char *host;
  char buf[MAX_LINE]; char in_buf[MAX_LINE*2]; // add a new array to manage input messgaes.
  int s;
  int len, buf_len; // buf_len in client.c so that client can receive messages.

  if (argc==2) {
    host = argv[1];
  }
  else {
    fprintf(stderr, "usage: simplex-talk host\n");
    exit(1);
  }

  /* translate host name into peer's IP address */
  hp = gethostbyname(host);
  if (!hp) {
    fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
    exit(1);
  }
  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
  sin.sin_port = htons(SERVER_PORT);
  
  /* active open */
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }
  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    perror("simplex-talk: connect");
    close(s);
    exit(1);
  }

  /* main loop: get and send lines of text */
  while (fgets(buf, sizeof(buf), stdin)) {
    buf[MAX_LINE-1] = '\0';
    len = strlen(buf) + 1;
    send(s, buf, len, 0);
    memset(in_buf, 0, MAX_LINE*2);
    /*Receive the server message and print it to stdout*/
    // If the client sends ">>> Ciao-Ciao" then close the client as well after closing the server.
    if (buf_len = recv(s, in_buf, sizeof(in_buf), 0)){
      char checkbuf[18];
      strncpy(checkbuf, in_buf, 17); checkbuf[17] = '\0';
      if(strcmp(checkbuf, "Server closing...") == 0){
        fprintf(stdout, "%s\n", "Server Closing...");
        close(s); exit(0);
      }
      fprintf(stdout, "%s\n", in_buf);
    }
  }

}