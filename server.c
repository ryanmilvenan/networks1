/* 
 * echoservert.c - A concurrent echo server using threads
 */

#include "nethelp.h"
#define CRLF "\r\n"

char conf[500];
void echo(int connfd);
void *thread(void *vargp);
char* portOpt, docRootOpt;
char** contentOpt, dirOpt;

void parse_option(char* line, char* target, char** option) {
    char *token;
    char separator[2] = " ";

    token = strtok(line, separator);
    printf("TOKEN: %s\n", token);
    printf("TARGET: %s\n", target);

    while( token != NULL ) {
        if(strcmp(token, target) == 0) {
            token = strtok(NULL, separator);
            printf("TOKEN2: %s\n", token); 
            while (token != NULL) {
                portOpt = token;
                token = strtok(NULL, separator);
            }
            break;
        }
        token = strtok(NULL, separator);
    }
}

void get_conf() {
    FILE *conffile;
    conffile = fopen ("ws.conf", "r");
    while (fgets (conf , 500, conffile)) {
        //Remove comments and whitespace
        if((strncmp(conf, "#", 1) == 0)||(strncmp(conf, "\n", 1) == 0)) continue;
        
        parse_option(conf, "Listen", portOpt);
        /*
        //Process options
        char *token;
        char separator[2] = " ";
        
        token = strtok(conf, separator);
        
        while( token != NULL){
            if(strcmp(token, "Listen") == 0) {
                token = strtok(NULL, separator);
                while (token != NULL) {
                    portOpt = token;
                    token = strtok(NULL, separator);
                }
                printf(" PORT OPT: %s\n", portOpt);
                break;
            }
            printf( " %s\n", token);

            token = strtok(NULL, separator);
        }
        */

        printf(" PORT OPT: %s\n", portOpt);
        //printf("%d. %s",index, conf);
    }
        //if (strcmp (conf, "") == 0){
        //        fgets (dir, 500, conffile);
        //        strtok(dir, "\n");
        //    }
        //if (strcmp (conf, "PORT=") == 0){
        //        fgets (port, 10, conffile);
        //        portf=atoi(port);
        //    }
        //if (strcmp (conf, "CHAR=") == 0){
        //        fgets (charset, 200, conffile);
        //        strtok(charset, "\n");
        //    }
        //if (strcmp (conf, "USER=") == 0){
        //        fgets (user, 100, conffile);
        //        strtok(user, "\n");
        //    }
        //if (strcmp (conf, "GRUP=") == 0){
        //        fgets (group, 100, conffile);
        //        strtok(group, "\n");
        //    }
        //} 
    fclose (conffile);
}


int main(int argc, char **argv) 
{
    int listenfd, *connfdp;
    char* port;
    unsigned int clientlen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid; 


    get_conf();
    /*
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = argv[1];

    listenfd = open_listenfd(port);
    while (1) {
        connfdp = malloc(sizeof(int));
        *connfdp = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
        pthread_create(&tid, NULL, thread, connfdp);
    }
    */
}

/* thread routine */
void * thread(void * vargp) 
{  
    int connfd = *((int *)vargp);
    pthread_detach(pthread_self()); 
    free(vargp);
    echo(connfd);
    close(connfd);
    return NULL;
}

/*
 * echo - read and echo text lines until client closes connection
 */
void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 

    while((n = readline(connfd, buf, MAXLINE)) != 0) {
	printf("server received %lu bytes\n", n);
	write(connfd, buf, n);
    }
}
