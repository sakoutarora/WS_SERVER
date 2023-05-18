#include <arpa/inet.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <unistd.h> 
#include <iostream>
#define PORT 8023

using namespace std; 


int backlogSize = 5;

int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) 
    {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }
    struct sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // bind to all available network interfaces
    serverAddress.sin_port = htons(PORT); // specify the port number

    int bindResult = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (bindResult == -1) {
        std::cerr << "Failed to bind socket." << std::endl;
        close(serverSocket);
        return -1;
    }
    std::cout << "==> server bind successful " << std::endl;

    int listenResult = listen(serverSocket, backlogSize);
    if (listenResult == -1) {
        std::cerr << "Failed to listen on socket." << std::endl;
        close(serverSocket);
        return -1;
    }

    struct sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);



    std::cout << "==> Waiting for incoming connection " << std :: endl;
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);   // works in blocking mode here 
    if (clientSocket == -1) {
        std::cerr << "Failed to accept incoming connection." << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "==> client connected with ip "<< clientAddress.sin_addr.s_addr << std::endl ;

    // receive data from client 
    char buffer[1024] = {'\0'}; 
    int bufferSize = sizeof(buffer);
    int counter = 0;
    char chbuf; 
    while (1)
    {
        int recvResult = recv(clientSocket, &chbuf, 1, 0);
        if (recvResult == -1) {
            std::cerr << "Failed to receive data: " << strerror(recvResult)<<std::endl;
            close(serverSocket);
            return -1;
        }
        if (counter >= 1024){
            counter = 0;
            std::cerr << "out of bound" << std::endl;
        }
        if (chbuf == '\t'){
            buffer[counter] = '\0';
            break;
        }
        else{
            buffer[counter] = chbuf; 
            counter++;
        }

    }
    

    std::cout << "Message received :"<<  buffer << std::endl;



    std::cout << "Yay !!! connection final " << std::endl; 

    close(serverSocket);
    return 0;
}