#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sstream>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define SOCKET int

#include <stdio.h>
#include <string.h>

bool stopSocketLoop = false;
SOCKET socket_peer;
bool externalCommandReady = false;

CommandInfo commandToolData;
bool sendToolAction = false;

struct SocData
{
    SOCKET* socket_peer;
};

CommandInfo externalCommand;

class SocketManager
{
    
    pthread_t socketThread;

public:
    bool createConn(int argc, char* argv[]);
    void startSocketLoop();
    void removeConn();
    void checkConn();
    static void* socketLoop(void* param);
    void sendCommand(CommandInfo commandData);
    
};

inline bool SocketManager::createConn(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: Polyland hostname port\n" << std::endl;
        return false;
    }

    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    
    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", errno);
        return 1;
    }

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
            address_buffer, sizeof(address_buffer),
            service_buffer, sizeof(service_buffer),
            NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer); 

    printf("Creating socket...\n");
    socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);

    if (socket_peer < 0) {
        fprintf(stderr, "socket() failed. (%d)\n", errno);
        return 1;
    }

    printf("Connecting...\n");
    if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)) {
        fprintf(stderr,"connect() failed. (%d)\n", errno);
        return 1;
    }
    freeaddrinfo(peer_address);

    printf("Connected\n");
    printf("To send data, enter text followed by enter.\n");

    return true;
}

inline void SocketManager::startSocketLoop()
{
    
    SocData socData;

    socData.socket_peer = &socket_peer;

    pthread_create(&socketThread, NULL, &SocketManager::socketLoop, &socData);
 
}

inline void SocketManager::removeConn()
{
    
}

inline void SocketManager::checkConn()
{
    
}

inline void SocketManager::sendCommand(CommandInfo commandData)
{
    
    string text = "EXTRUDE " + to_string(commandData.polyID) + " " + to_string(commandData.amount);
    sendToolAction = true;
    const char* read = text.c_str();
    int bytes_send = send(socket_peer, read, strlen(read), 0);
}

void* SocketManager::socketLoop(void* param) 
{

    while(1)
    {
        std::cout << "  " << std::endl;
        sleep(1);
        
        fd_set reads;
        
        FD_ZERO(&reads);
        
        FD_SET(socket_peer, &reads);
        
        FD_SET(0, &reads);
        
        if (select(socket_peer+1, &reads, 0, 0, NULL) < 0) {
            fprintf(stderr, "select() Failed. (%d)\n", errno);
            return;
        }

        if (FD_ISSET(socket_peer, &reads)) {
            char read[4096];
            int bytes_received = recv(socket_peer, read, 4096, 0);
            if (bytes_received < 1) {
                printf("Connection closed by peer!\n");
                break;
            }
            string text = read;
            stringstream ss(text);
            string toolType;
            unsigned int polyID = {};
            float amount{};
            ss >> toolType >> polyID >> amount;
            
            externalCommand.toolName = toolType;
            externalCommand.polyID = polyID;
            externalCommand.amount = amount;
            externalCommand.ready = true;
            externalCommand.meshID = 0;
            externalCommand.order = "SECOND";

            externalCommandReady = true;

            std::cout <<"Tooltype: "<< toolType << " polyID: " << polyID << " amount: " << amount << std::endl; 

        }        

        if (stopSocketLoop == true)
            break;
    }
}
#endif