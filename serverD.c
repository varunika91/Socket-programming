/*
 ** ServerD code
 ** Reference: Beej network programming guide
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define MYPORT "24658"    // the static UDP port
#define PORT "25658" // the static TCP port of the Client 

#define MAXDATASIZE 100 // max number of bytes we can get at once 
#define MAXBUFLEN 100
// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

in_port_t get_in_port(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return (((struct sockaddr_in*)sa)->sin_port);
	}
	return (((struct sockaddr_in6*)sa)->sin6_port);
}

int main() {
	
	int value;
	char server_no[10];
	char buf1[128] = {};
	struct sockaddr_storage their_addr;
	int buf_udp[4][4];
	socklen_t addr_len;
	int x, y;
	int sockfd, numbytes;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET_ADDRSTRLEN];

	//Starting the TCP client code for ServerD
	printf("The Server D is up and running.\n");
	printf("The Server D has the following neighbor information:\n");

	//Reading the conents of the file containing neighbour information
	FILE *fp = fopen("serverD.txt", "r");
	if (fp != NULL) {
		char line [128]; /* or other suitable maximum line size */
		while (fgets(line, sizeof line, fp) != NULL) /* read a line */ {
			sscanf(line, "%s %d", server_no, &value);
			strcat(buf1, line);
		}
		fclose(fp);
	}
	//Printing the neighbour information
	printf("Neighbor Cost\n");
	printf("%s",buf1);


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo("nunki.usc.edu", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "failed to connect\n");
		return 2;
	}

	//Sending neighbour information to the Client
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr),s, sizeof s);
	freeaddrinfo(servinfo); // all done with this structure

	if (send(sockfd, buf1, 128, 0) == -1)
		perror("send");

	printf("The Server D finishes sending its neighbor information to the Client with TCP port number %d and IP address %s(Client's TCP port number and IP address).\n",ntohs(get_in_port(p->ai_addr)),s);
	struct sockaddr_in my_addr;
	socklen_t len = sizeof(my_addr);
	if((getsockname(sockfd,(struct sockaddr *)&my_addr,&len))==-1)
		perror("getsockname");
	printf("For this connection with the Client, the Server D has TCP port number %d and IP address %s\n\n",ntohs(my_addr.sin_port),inet_ntoa(my_addr.sin_addr));
	close(sockfd);


	//Starting UDP server code

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo("nunki.usc.edu", MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	addr_len = sizeof their_addr;

	//Recieve the adjacency matrix from the Client via UDP
	if ((numbytes = recvfrom(sockfd, buf_udp, sizeof(buf_udp) , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}


	//Print and calculate the neighbour information recived from the Client
	inet_ntop(their_addr.ss_family,	get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
	printf("The server D has received the network topology from the Client with UDP port number %d and IP address %s(Client's UDP port number and IP address) as follows:\n",ntohs(get_in_port((struct sockaddr *)&their_addr)),s);

	printf("Edge	Cost\n");
	for (x = 0; x <= 3; x++) {
		for (y = 0; y <= 3; y++) {

			if(buf_udp[x][y]!=0){
				if(x==0)
					printf("A");
				else if (x==1)
					printf("B");
				else if (x==2)	
					printf("C");
				else 
					printf("D");
			}
			if(buf_udp[x][y]!=0){
				if(y==0)
					printf("A\t");
				else if (y==1)
					printf("B\t");
				else if (y==2)	
					printf("C\t");
				else 
					printf("D\t");
				printf("%d\n",buf_udp[x][y]);
			}

		}

	}
	//struct sockaddr_in my_addr;
	//socklen_t len = sizeof(my_addr);
	if((getsockname(sockfd,(struct sockaddr *)&my_addr,&len))==-1)
		perror("getsockname");
	printf("For this connection with the Client, the Server D has UDP port number %d and IP address %s\n",ntohs(my_addr.sin_port),inet_ntoa(my_addr.sin_addr));

	close(sockfd);

	return 0;
}
