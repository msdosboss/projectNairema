#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "networking.h"


char *inputPort(int argc, char *argv[]){
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"--port") == 0 || strcmp(argv[i], "-p") == 0){
            i++;
            int port = atoi(argv[i]);
            if(port == 0){
                printf("that is not a valid port\n");
                return NULL;
            }
            return argv[i];
        }
    }
}
