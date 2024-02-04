#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "networking.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define BUFFLEN 512

char *port(int argc, char *argv[]){
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"--port") == 0 || strcmp(argv[i], "-p") == 0){
            i++;
            port = atoi(argv[i]);
            if(port == 0){
                printf("that is not a valid port\n");
                return NULL;
            }
            return argv[i];
        }
    }
}

int main(int argc, char *argv[]){
    const char *defaultPort = "5456";
    char *port = defaultPort;
    if(agrc > 1){
        if(port = port(argc, argv) == NULL){
            return -1
        }
    }
    WSADATA wsaData;
    SOCKET connectSocket = INVAILD_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    const char *buf = "window socket send";
    char recvbuf[BUFFLEN];
    int iResult;
    int recvbuflen = BUFFLEN;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult){
        printf("error in WSAStartup");
        return iResult;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    //resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", port, &hints, &result);
    if(iResult){
        printf("getaddrinfo failed check what the program returned\n");
        WSACleanup();
        return iResult;
    }

    for(ptr = result; ptr != NULL; ptr = ptr->ai_next){
        //create socket for connecting to server
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype);
        if(connectSocket == INVAILD_SOCKET){
            printf("socket failed to create");
            WSACleanup();
            return 1;
        }

        //connect to server
        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen, ptr->ai_protocol);
        if(iResult == SOCKET_ERROR){
            closesocket(connectSocket);
            connectSocket = INVAILD_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    
    if(connectSocket == INVAILD_SOCKET){
        printf("Unable to connect to server");
        WSACleanup();
        return 1;
    }
    
    //Send an initial buffer
    iResult = send(connectSocket, buf, (int)strlen(buf), 0);
    if(iResult == SOCKET_ERROR){
        printf("failed to send");
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    printf("Byte sent: %ld\n", iResult);

    //shutdown the connection since no more data will be sent
    iResult = shutdown(connectSocket, SD_SEND)
    if(iResult == SOCKET_ERROR){
        printf("shutdown error\n");
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    do{
        iResult = recv(connectSocket, recvbuf, recvbuflen, 0);
        if(iResult > 0){
            printf("recivied: %s from server\n", recvbuf);
        }
        else if(iResult == 0){
            printf("connection closed\n");
        }
        else{
            printf("recv error\n");
        }
    } while(iResult > 0)


    //clean up
    closesocket(connectSocket);
    WSACleanup();

    return 0;
}