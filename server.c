/* 
 * echoservert.c - A concurrent echo server using threads
 */

#include "nethelp.h"
#include "config-parse.h"
#include "server.h"
#define CRLF "\n"
#define MAX_DATA 512

char conf[MAX_DATA];
void echo(int connfd);
void *thread(void *vargp);
char *portOpt = NULL, *docRootOpt = NULL;
char **contentOpt = NULL, **contentLookup = NULL, **dirOpt = NULL;
int contentOptCount = 0, dirOptCount = 0;


void get_conf() {
    FILE *conffile;
    conffile = fopen ("ws.conf", "r");
    int index = 0;
    while (fgets (conf , 500, conffile)) {
        //Remove comments and whitespace
        if((strncmp(conf, "#", 1) == 0)||(strncmp(conf, "\n", 1) == 0)) continue;
        
        if(portOpt == NULL) {
            char* portOption = parse_option(conf, "Listen");
            portOpt = strdup(portOption);
            printf("PORT OPT: %s\n", portOpt);
            continue;
        }
        
        if(docRootOpt == NULL) {
            char* docOption = parse_option(conf, "DocumentRoot");
            docRootOpt = strdup(docOption);
            printf("Doc Root OPT: %s\n", docRootOpt);
            continue;
        }

        if(dirOpt == NULL) {
            int line_length = strlen(conf);
            char line_cpy[line_length + 1];
            strcpy(line_cpy, conf);
            
            dirOpt = parse_dir_options(conf, "DirectoryIndex"); 
            dirOptCount = get_num_options(line_cpy);
            for(int i = 0; i < dirOptCount; i++) {
                printf("DirIndex %d: %s\n", i, dirOpt[i]);
            }
            continue;
        }

        if(contentLookup == NULL) {
            contentLookup = malloc(MAX_DATA * sizeof(char*));
            contentOpt = malloc(MAX_DATA * sizeof(char*));
        }

        contentOptCount = parse_content_options(conf, contentLookup, contentOpt);

        if(contentLookup != NULL) {
            printf( "Content %d: %s Type: %s\n", index, contentLookup[index], contentOpt[index]);
            index++;
        }
    }
    fclose (conffile);
}

void destroy_conf() {
    free(portOpt);
    free(docRootOpt);

    for(int i = 0; i < dirOptCount; i++) {
        free(dirOpt[i]);
    }
    free(dirOpt);
    
    for(int i = 0; i < contentOptCount; i++) {
        free(contentLookup[i]);
        free(contentOpt[i]);
    }
    free(contentLookup);
    free(contentOpt);
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
    */
    port = portOpt;
    
    listenfd = open_listenfd(port);
    while (1) {
        connfdp = malloc(sizeof(int));
        *connfdp = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
        pthread_create(&tid, NULL, thread, connfdp);
    }
    

    destroy_conf();
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
    bool keep_alive = false;
    char *request_type = NULL;
    char request_dir[MAXLINE];
    char error[MAXLINE];
    RETURN_TYPE code = -1;

    while((n = readline(connfd, buf, MAXLINE)) != 0) {
	printf("server received %lu bytes\n", n);
        printf("received: %s", buf);
        if(strcmp(buf, CRLF) == 0) {
            if(keep_alive) {
            
            } else {
                if(request_type != NULL) {
                    free(request_type);
                }
                return;
            }
        }
        
        //Parse request
        if(request_type == NULL) {
            request_type = malloc(5 * sizeof(char));
            code = parse_initial_line(buf, request_type, request_dir, error);
        } else {
        
        } 


        //Respond
	write(connfd, buf, n);
    }
}

RETURN_TYPE parse_initial_line(char* line, char* request_type, char* request_dir, char* error) {
    char* token;
    char separator[3] = " \n";

    token = strtok(line, separator);
    if(strcmp(token, "GET") != 0) {
        strncpy(error, token, (strlen(token) + 1));
        return BAD_REQUEST_METHOD;
    } else {
        request_type = token;
        token = (NULL, separator);
    }

    printf("About to parse directory");
    if(is_directory(token) != 0) {
        printf("BadDirectory");
        strncpy(error, token, (strlen(token) + 1)); 
        return BAD_REQUEST_URI;
    } else {
        strncpy(request_dir, token, (strlen(token) + 1));
        token = (NULL, separator);
    }

    return OK;

}

int is_directory(char* extension) {
//    char path[MAXLINE];
/*
    memset(path, 0, sizeof(path));
    strcpy(path, docRootOpt);
    strcat(path, extension);
    */
    if (0 != access("~/Documents/grad/networks/", F_OK)) {
      if (ENOENT == errno) {
          return -1;
      }

      if(ENOTDIR == errno) {
          return -1;
      }
    }
    printf("DIRECTORY EXISTS");
    return 0;
}
