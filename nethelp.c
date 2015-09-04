#include "nethelp.h"

/* 
 * open_listenfd - open and return a listening socket on port
 * Returns -1 in case of failure 
 */
int open_listenfd(char *port) 
{
    int listenfd, rv, optval=1;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server:socket");
            continue;
        }

        if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(listenfd);
            perror("socket:bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if(p == NULL) {
        fprintf(stderr, "server failed to bind\n");
    }

    if(listen(listenfd, LISTENQ) < 0) {
        perror("listen");
        exit(1);
    }

    return listenfd;
} /* end open_listenfd */

/*
 * open_clientfd - open connection to server at <hostname, port>
 *   and return a socket descriptor ready for reading and writing.
 *   Return <0 in case of failure.
 */
int open_clientfd(char *hostname, char *port)
{
    int clientfd;

    int status, rv = 0;
    struct addrinfo hints;
    struct addrinfo *servinfo, *p;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    /* Fill in the server's IP address and port */
    if ((status = getaddrinfo(hostname, port, &hints, &servinfo )) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next){
        if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        
        if(connect(clientfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(clientfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    return clientfd;
} /* end open_clientfd */


/*
 * readline - read a line of text
 * return the number of characters read
 * return -1 if error
 */
int readline(int fd, char * buf, int maxlen)
{
  int nc, n = 0;
  for(n=0; n < maxlen-1; n++)
    {
      nc = read(fd, &buf[n], 1);
      if( nc <= 0) return nc;
      if(buf[n] == '\n') break;
    }
  buf[n+1] = 0;
  return n+1;
}

