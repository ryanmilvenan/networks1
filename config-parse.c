#include "nethelp.h"
#define MAX_DATA 512

char* parse_option(char* line, char* target) {
    char *token;
    char *option = NULL;
    char separator[3] = " \n";

    token = strtok(line, separator);
    

    while( token != NULL ) {
        if(strcmp(token, target) == 0) {
            token = strtok(NULL, separator);
            while (token != NULL) {
                option = token;
                token = strtok(NULL, separator);
            }
            break;
        }
        token = strtok(NULL, separator);
    }

    return option; 
}

int get_num_options(char* line) {
    char *token;
    char separator[3] = " \n";
    int index = 0;

    token = strtok(line, separator);
    while( token != NULL) {
        token = strtok(NULL, separator);
        if(token == NULL) break;
        int length = strlen(token);
        if(length > 1) {
            index++;
        } else {
            break;
        }
    }

    return index;
}

char** parse_dir_options(char* line, char* target) {
    char *token;
    char separator[2] = " ";
    char **options_array = NULL;
    int num_options = 0;
    int line_length = strlen(line);
    char line_cpy[line_length + 1];
    strcpy(line_cpy, line);

    num_options = get_num_options(line_cpy);
    options_array = malloc(num_options * sizeof(char *));

    if(options_array != NULL){
        token = strtok(line, separator);
        if(strcmp(token, target) == 0) {
            int index = 0;
            while(index < num_options){
                token = strtok(NULL, separator);
                int token_length = strlen(token);
                token_length++;
                options_array[index] = malloc(token_length * sizeof(char));
                strcpy(options_array[index], token);
                index++;
            }
        }
    }

    return options_array;
}

int parse_content_options(char* line, char **options_lookup, char **options_array) {
    char *token;
    char separator[3] = " \n";
    int index = 0;

    if(options_array != NULL){
        while(options_array[index] != NULL) {
            index++;
        }

        token = strtok(line, separator);
        if((strncmp(line, ".", 1) == 0)) {
            int token_length = strlen(token);        
            token_length++;
            options_lookup[index] = malloc(token_length * sizeof(char));
            strcpy(options_lookup[index], token);
        }
        token = strtok(NULL, separator);
        if(token != NULL) {
            int token_length = strlen(token);
            token_length++;
            options_array[index] = malloc(token_length * sizeof(char));
            strcpy(options_array[index], token);
        }
    }
    return index;
}

