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
	struct sockaddr_in my_addr, my_addr1;
	int client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) {
        std::cerr << "Error in client creating\n";
    }
	else {
        std::cout << "Client Created" << ln;
    } 		
		
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(PORT);
	

	my_addr.sin_addr.s_addr = inet_addr(ADDRESS);

	my_addr1.sin_family = AF_INET;
	my_addr1.sin_addr.s_addr = INADDR_ANY;
	my_addr1.sin_port = htons(12010);
	

	my_addr1.sin_addr.s_addr = inet_addr(ADDRESS);
	if (bind(client, (struct sockaddr*) &my_addr1, sizeof(struct sockaddr_in)) == 0) {
        std::cout << "Binded Correctly" << ln;
    }
	else {
        std::cerr << "Unable to bind\n";
    }
	socklen_t addr_size = sizeof my_addr;
    int con = connect(client, (struct sockaddr*) &my_addr, sizeof my_addr);
    while (1) {
        std::string message = "ping";
        ssize_t bytesWriten = send(client,  message.c_str(), message.length(), 0);
        if (bytesWriten <= 0) {
            std::cerr << "Writing error." << ln;
            return 1;
        }
        
        sleep(1);
        ssize_t bytesRead = recv(client, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Reading error." << ln;
            return 1;
        } 
        
        message = buffer;
        memset(buffer, '\0', sizeof(buffer));
        std::cout << "Server: " << message << ln;
    }
	
	return 0;
}
