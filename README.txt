a) Full Name: Varunika Gupta

b) Student ID: 8023072658

c) What I have done in the assignment: 
 The socket programming code has been developed to simulate a system based on Djikstra algorithm which builds a map of the network topology.
 The network consists of 1 Client machine and 4 Server machines. The server machines read a file which contains the information about the neighbours they are connected to.
 The neighbour information is shared by the 4 Servers with the Client via TCP connection. Here the Client machine acts like a TCP server (with static TCP port) and all the Servers behave like TCP Clients (with dynamic TCP ports).
 The Client receives the network information from all the servers and combines it to form the topology.
 The Client then initiates a UDP session with all the Servers and sends the collated information to all the servers.
 Now, the Client behaves like a UDP client( having 4 dynamic UDP ports for each of the servers) and the each of the 4 Servers behave like UDP server (each with a static UDP port).
 Once the information is sent to all the servers, the client calculates the minimum spanning tree of the network and displays it on the terminal of client. 
 servers choose dynamic TCP port to send the data and client uses dynamic UDP port to send the data.

d) Names of code files and what they do.
 	1) serverA.c: 
	This Server machine reads a file containing the neighbour information it is connected to. This is done via TCP connection.
	It then establishes a TCP connection with the Client and sends this information to the Client. 
	Once the information is sent, it opens UDP port (acting like a UDP server)and then waits for the connection from the Client (UDP Client) so that the client can send the network topology to it.

	2) serverB.c: 
	This Server machine reads a file containing the neighbour information it is connected to. This is done via TCP connection.
	It then establishes a TCP connection with the Client and sends this information to the Client. 
	Once the information is sent, it opens UDP port (acting like a UDP server)and then waits for the connection from the Client (UDP Client) so that the client can send the network topology to it.

  	3) serverC.c: 
	This Server machine reads a file containing the neighbour information it is connected to. This is done via TCP connection.
	It then establishes a TCP connection with the Client and sends this information to the Client. 
	Once the information is sent, it opens UDP port (acting like a UDP server)and then waits for the connection from the Client (UDP Client) so that the client can send the network topology to it.

  	4) serverD.c: 
	This Server machine reads a file containing the neighbour information it is connected to. This is done via TCP connection.
	It then establishes a TCP connection with the Client and sends this information to the Client. 
	Once the information is sent, it opens UDP port (acting like a UDP server)and then waits for the connection from the Client (UDP Client) so that the client can send the network topology to it.

	5) client.c: 
	This sets up a TCP port and listens for connections from the TCP clients(i.e. 4 servers). 
	Once the connection is established, it receives the neighbour information from all the 4 servers and builds the topology.
	This network topology information is then sent via UDP connection to all the servers.
	After sending this information, it calculates the minimum spanning tree and prints the output in the client terminal.

e) Please type the following commands in the commandline to clean and compile all the files and make the running executables. 
   Please open 5 different terminals and run each of the below commands in a separate terminal in the below mentioned order.

	Step 1: make clean
	Step 2: make socket
	Step 3: Type the following commands in separate terminals 
		1) ./client
		2) ./serverA
	    3) ./serverB
		4) ./serverC
		5) ./serverD
		

f) The format of all messages(Phase 1,2 and 3) are compliant to the specification.


g) No Idiosyncrancy. The project doesnot fail in any conditions mentioned in specification. 


h) The TCP and UDP code from Beej Networking Tutorial has been reused. The code for Minimum Spanning tree has been modified from www.c-program-example.com
