/*
Run client first and then server
*/

#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>

int main(int argc, char const* argv[])
{
	// create server socket similar to what was done in
	// client program
	int clntSockD = socket(AF_INET, SOCK_STREAM, 0);

	// string store data to send to client
	
	char clntMsg[255];
	char ch;
	int ind = 0;
	int alp,vow,dig,spcl;
	printf("Enter multiple line message to pass[terminate with'.']:\n");
	
	while(ch!='.'){
		scanf("%c",&ch);
		clntMsg[ind++]=ch;
	}
	clntMsg[ind] = '\0';
	
	//scanf("%[^\n]",clntMsg);

	// define server address
	struct sockaddr_in clntAddr;

	clntAddr.sin_family = AF_INET;
	clntAddr.sin_port = htons(9001);
	clntAddr.sin_addr.s_addr = INADDR_ANY;

	// bind socket to the specified IP and port
	bind(clntSockD, (struct sockaddr*)&clntAddr,sizeof(clntAddr));

	// listen for connections
	listen(clntSockD, 1);

	// integer to hold client socket.
	int serverSocket = accept(clntSockD, NULL, NULL);

	// send's messages to client socket
	send(serverSocket, clntMsg, sizeof(clntMsg), 0);
	
	//int counts[5];
	//recv(serverSocket, counts, sizeof(counts), 0);
	//printf("Alphabets: %d, Vowels: %d, Digits: %d, Special Characters: %d",counts[0],counts[1],counts[2],counts[3]);

	recv(serverSocket, &alp, sizeof(alp), 0);
	recv(serverSocket, &vow, sizeof(vow), 0);
	recv(serverSocket, &dig, sizeof(dig), 0);
	recv(serverSocket, &spcl, sizeof(spcl), 0);
	printf("\nAlphabets: %d, Vowels: %d, Digits: %d, Special Characters: %d\n",alp,vow,dig,spcl);

	return 0;
}

