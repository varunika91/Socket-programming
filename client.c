/*
 ** server.c -- a stream socket server demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "25658"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold
#define MAXDATASIZE 128 // max number of bytes we can get at once

void sigchld_handler(int s) {
	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

in_port_t get_in_port(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return (((struct sockaddr_in*)sa)->sin_port);
	}
	return (((struct sockaddr_in6*)sa)->sin6_port);
}

// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

int find(int);
int uni(int,int);
int i,a,b,u,v,n,ne=1;
int min,mincost=0,cost[9][9],parent[9];

int main(void) {
	int sockfd, sockfdu, new_fd, numbytes; // listen on sock_fd, new connection on new_fd
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	buf[numbytes] = '\0';
	int adj_mat[4][4] = {{0}};
	int x, y, j;
	char *token;
	int row1;
	int count, counter, link_cost;
	char serv_name[10];
	count = 0;
	link_cost = 0;
	int serv_num;
	row1 = 0;
	counter = 0;
	char char1;
	struct sockaddr_in addr_1;
	socklen_t len=sizeof(addr_1);
	struct sockaddr_in addr_2;
	socklen_t len_2=sizeof(addr_2);
	//Variables for MST


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;//IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo("nunki.usc.edu", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof (int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}
	if((getsockname(sockfd,(struct sockaddr *)&addr_1,&len))==-1)
		perror("getsockname");
	printf("The Client has TCP port number %d and IP address %s.\n\n",ntohs(addr_1.sin_port),inet_ntoa(addr_1.sin_addr));

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}
	for (j = 0; j <= 3; j++) {
		if (listen(sockfd, BACKLOG) == -1) {
			perror("listen");
			exit(1);
		}

		sa.sa_handler = sigchld_handler; // reap all dead processes
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		if (sigaction(SIGCHLD, &sa, NULL) == -1) {
			perror("sigaction");
			exit(1);
		}

		while (counter <= 3) { // main accept() loop
			sin_size = sizeof their_addr;
			new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);

			if (new_fd == -1) {
				perror("accept");
				continue;
			}
			if(counter==0)
				char1='A';
			else if (counter==1)
				char1='B';
			else if (counter==2)
				char1='C';
			else
				char1='D';
			/*inet_ntop(their_addr.ss_family,
                    get_in_addr((struct sockaddr *) &their_addr),
                    s, sizeof s);*/

			inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
			printf("The Client receives neighbor information from the Server %c with TCP port number %d and IP address %s (The Server %c TCP port number and IP address)\n",char1,ntohs(get_in_port((struct sockaddr *)&their_addr)),s,char1);
			
			if ((numbytes = recv(new_fd, buf, MAXDATASIZE - 1, 0)) == -1) {
				perror("recv");
				exit(1);
			}
			printf("The Server %c has the following neighbor information:\nNeighbor Cost\n",char1);
			printf("%s", buf);

			//printf("For this connection with Server %c, The Client has TCP port number %s and IP address %s\n",char1,ntohs(addr_1.sin_port),inet_ntoa(addr_1.sin_addr));

			token = strtok(buf, "\n"); //get the first token

			while (token != NULL) {
				sscanf(token, "%s %d", serv_name, &link_cost);

				if (strcmp(serv_name, "serverA") == 0) {
					serv_num = 0;

				} else if (strcmp(serv_name, "serverB") == 0) {

					serv_num = 1;

				} else if (strcmp(serv_name, "serverC") == 0) {

					serv_num = 2;

				} else {

					serv_num = 3;
				}
				token = strtok(NULL, "\n");
				if(serv_num>row1)
					adj_mat[row1][serv_num] = link_cost;

			} //end of while

			row1++;

			if((getsockname(sockfd,(struct sockaddr *)&addr_1,&len))==-1)
				perror("getsockname");
			printf("For this connection with Server %c, The Client has TCP port number %d and IP address %s.\n\n",char1,ntohs(addr_1.sin_port),inet_ntoa(addr_1.sin_addr));

			counter++;
			//printf("client: received '%s'\n",buf);
			//close(new_fd);  // parent doesn't need this
			//close(sockfd);
		}//end of while
	}// end of for
	int w;
	const char* serv_port;
	char char2;

	//Starting udp code
	for(w=0;w<=3;w++)
	{
#define p0 "21658"
#define p1 "22658"
#define p2 "23658"
#define p3 "24658"
		usleep(100000);
		switch(w){
		case 0: serv_port=p0;
		char2='A';
		break;
		case 1: serv_port=p1;
		char2='B';
		break;
		case 2: serv_port=p2;
		char2='C';
		break;
		case 3: serv_port=p3;
		char2='D';
		break;
		}

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;

		if ((rv = getaddrinfo("nunki.usc.edu", serv_port, &hints, &servinfo)) != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			return 1;
		}
		//..for for 4
		// loop through all the results and make a socket
		for (p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfdu = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
				perror("talker: socket");
				continue;
			}
			break;
		}

		if (p == NULL) {
			fprintf(stderr, "talker: failed to create socket\n");
			return 2;
		}
		/*printf("Before sending");
    for (x = 0; x <= 3; x++) {
        for (y = 0; y <= 3; y++) {

            printf("%d \t", adj_mat[x][y]);
        }
        printf("\n");
    }*/
		if ((numbytes = sendto(sockfdu, adj_mat, sizeof (adj_mat), 0,
				p->ai_addr, p->ai_addrlen)) == -1) {
			perror("talker: sendto");
			exit(1);
		}
		char s1[INET_ADDRSTRLEN];
		inet_ntop(p->ai_family,
           	get_in_addr((struct sockaddr *) p->ai_addr),s1, sizeof s1);
		printf("The Client has sent the network topology to the network topology to the Server %c with UDP port number %s and IP address %s (Server %c UDP port number and IP address) as follows:\n",char2,serv_port,s1,char2);
		printf("Edge	Cost\n");
		for (x = 0; x <= 3; x++) {
			for (y = 0; y <= 3; y++) {

				if(adj_mat[x][y]!=0){
					if(x==0)
						printf("A");
					else if (x==1)
						printf("B");
					else if (x==2)
						printf("C");
					else
						printf("D");
				}
				if(adj_mat[x][y]!=0){
					if(y==0)
						printf("A\t");
					else if (y==1)
						printf("B\t");
					else if (y==2)
						printf("C\t");
					else
						printf("D\t");
					printf("%d\n",adj_mat[x][y]);
				}
				//printf("%d \t", adj_mat[x][y]);
			}
			//printf("\n");
		}
		if((getsockname(sockfd,(struct sockaddr *)&addr_2,&len_2))==-1)
				perror("getsockname");
		printf("For this connection with Server %c, The Client has TCP port number %d and IP address %s.\n\n",char2,ntohs(addr_2.sin_port),inet_ntoa(addr_2.sin_addr));

		freeaddrinfo(servinfo);

		//printf("talker: sent %d bytes to %s\n", numbytes, "localhost");
		//printf("Hello");
		close(sockfdu);
	}	//End of for
	printf("\nThe Client has calculated a tree\n");

	n=4;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++)
		{ //cost[i][j]=adj_mat[i][j];
			if(adj_mat[i][j]==0)
				adj_mat[i][j]=999;
		}
	}

	int mat[4][4]={{0}};
	int row=0;
	int column=0;
	while(ne<n){
		for(i=0,min=999;i<n;i++) {
			for(j=0;j<n;j++){
				if(adj_mat[i][j]<min)
				{
					min=adj_mat[i][j];
					a=u=i;
					b=v=j;
				}
			}
		}
		u=find(u);
		v=find(v);
		if(uni(u,v)) {
			ne++;
			if(a==0){
				row=0;}
			else if (a==1){
				row=1;}
			else if (a==2){
				row=2;}
			else {
				row=3;}
			if(b==0){
				column=0;}
			else if (b==1){
				column=1;}
			else if (b==2){
				column=2;}
			else {
				column=3;}
			//printf("\t%d\n",min);
			mat[row][column]=min;
			mincost +=min;
		}
		adj_mat[a][b]=adj_mat[b][a]=999;
	}
	printf("\nThe tree cost is (cost of the tree): %d\n\n",mincost);
	printf("Edge----Cost\n");
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++){
			if(mat[i][j]!=0){
				if(i==0)
					printf("A");
				else if (i==1)
					printf("B");
				else if (i==2)
					printf("C");
				else
					printf("D");
				if(j==0)
					printf("A");
				else if (j==1)
					printf("B");
				else if (j==2)
					printf("C");
				else
					printf("D");
				printf("\t%d\n",mat[i][j]);
			}
		}
	}
	return 0;
}
int find(int i)
{
	while(parent[i])
		i=parent[i];
	return i;
}
int uni(int i,int j)
{
	if(i!=j)
	{
		parent[j]=i;
		return 1;
	}
	return 0;
}
