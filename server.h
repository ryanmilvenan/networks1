#include "nethelp.h"
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

//Response Values
typedef enum RETURN_TYPE { OK, BAD_REQUEST_METHOD, BAD_REQUEST_URI, BAD_REQUEST_HTTP, NOT_FOUND, NOT_IMPLEMENTED, SERVER_ERR } RETURN_TYPE;

FILE *confile;
RETURN_TYPE parse_initial_line(char* line, char* request_type, char* request_dir, char* error);
int is_directory(char* extension); 


