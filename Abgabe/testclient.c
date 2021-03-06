#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNUMMER 8110

int main(int argc, char *argv[]) {
	char buffer[1024];
	int wert, sockfd, n;
	struct sockaddr_in servaddr;
	char *host = argv[1];
	char *nachricht = (char*)calloc(1024, sizeof(char));
	
	if (inet_aton(host, &servaddr.sin_addr)==0) {
		perror("inet_aton"); exit(1);
	}
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORTNUMMER);
	
	printf("PORT: %d\n", servaddr.sin_port);
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0) {
		perror("socket"); exit(-1);
	}
	
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) <0) {
		perror("connect"); exit(-1);
	}
	
	memset(&buffer, 0, 1024);
	
	strcpy(nachricht, "USER joendhard\r\n");
	write(sockfd, nachricht, strlen(nachricht));
	n = read(sockfd, buffer, 1024);
	printf("Buffer: %s", buffer);
	
	memset(&buffer, 0, 1024);
	
	strcpy(nachricht, "PASS biffel\r\n");
	write(sockfd, nachricht, strlen(nachricht));
	n = read(sockfd, buffer, 1024);
	printf("Buffer: %s", buffer);
	
	memset(&buffer, 0, 1024);
	
	strcpy(nachricht, "STAT\r\n");
	write(sockfd, nachricht, strlen(nachricht));
	n = read(sockfd, buffer, 1024);
	printf("Buffer: %s", buffer);
	
	strcpy(nachricht, "LIST\r\n");
	write(sockfd, nachricht, strlen(nachricht));
	n = read(sockfd, buffer, 1024);
	printf("Buffer: %s", buffer);
	
	memset(&buffer, 0, 1024);
	
	strcpy(nachricht, "LIST 2\r\n");
	write(sockfd, nachricht, strlen(nachricht));
	n = read(sockfd, buffer, 1024);
	printf("Buffer %s", buffer);
	
	memset(&buffer, 0, 1024);
	
	strcpy(nachricht, "RETR 8\r\n");
	write(sockfd, nachricht, strlen(nachricht));
	read(sockfd, buffer, 1024);
	printf("Buffer: %s", buffer);
	memset(&buffer, 0, 1024);
	
	
	close(sockfd);
	
	return 0;
}
