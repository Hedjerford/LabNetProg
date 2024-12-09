#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ADDRESS "127.0.0.0"
#define PORT 2024
#define LEN 200
#define ln '\n'

int main()
{

	char buffer[LEN];
    memset(buffer, '\0', sizeof(buffer));
	int server = socket(AF_INET, SOCK_STREAM, 0);
	if (server < 0)
		printf("Error in server creating\n");
	else
		printf("Server Created\n");
		
	struct sockaddr_in my_addr, peer_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	

	my_addr.sin_addr.s_addr = inet_addr(ADDRESS);
	
	my_addr.sin_port = htons(PORT);

	if (bind(server, (struct sockaddr*) &my_addr, sizeof(my_addr)) == 0) {
        std::cout << "Binded Correctly." << ln;
    }
	else {
        std::cerr << "Unable to bind.\n";
        return -1;
    }
		
	if (listen(server, 2) == 0) {
        std::cout << "Listening ..." << ln;
    }
	else {
        std::cerr << "Unable to listen\n";
        return -1;
    }	
	
	socklen_t addr_size;
	addr_size = sizeof(struct sockaddr_in);
	

    int acc = accept(server, (struct sockaddr*) &peer_addr, &addr_size);
	std::cout << "Connection Established." << ln;

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN);
	
	std::cout << "connection established with IP : " << ip << " and PORT : " << ntohs(peer_addr.sin_port) << ln;

	while (1)
	{
        std::string message;
        ssize_t bytesRead = recv(acc, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Reading error." << ln;
            return 1;
        }
        message = buffer;
        memset(buffer, '\0', sizeof(buffer));
		std::cout << "Client: " << message << ln;
        sleep(1);
        message = "pong";
        ssize_t bytesWriten = send(acc,  message.c_str(), message.length(), 0);
        if (bytesWriten <= 0) {
            std::cerr << "Writing error." << ln;
            return 1;
        }
	} 
	return 0;
}
