#include <arpa/inet.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <unistd.h> 
#include <iostream>
#include <stdint.h>

#define PORT 8023


int main(){

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( clientSocket == -1 ){
        std::cerr << "error couldnt connect to client" << std::endl;
    } 
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT); // specify the port number

    int connec_result  =  connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connec_result == -1){
        std::cerr << "ERROR in connection " << std::endl;
        close(clientSocket);
    }
    const char* message = "server!\t";
    std::cout << "sending message " << message << " of len " << strlen(message) << std::endl;
    int sendResult = send(clientSocket, message, strlen(message), 0);
    if (sendResult == -1) {
        std::cerr << "Failed to send data." << std::endl;
        close(clientSocket);
        return -1;
    }
    std::cout << "Data sent successfully " << std::endl;
    close(clientSocket);
    return 0;
}