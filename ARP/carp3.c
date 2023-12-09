#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int cl_fd,n,c;
	struct sockaddr_in cl_add;
	char msg[256];
	char ip[256];
	char mac[256];
	strcpy(ip,argv[2]);
	strcpy(mac,argv[3]);
	
	cl_add.sin_family=AF_INET;
	cl_add.sin_addr.s_addr=INADDR_ANY;
	cl_add.sin_port=htons(atoi(argv[1]));
	
	if ((cl_fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("\nSocket Failure");
		exit(1);
	}
	
	connect(cl_fd,(struct sockaddr*) &cl_add,sizeof(cl_add));
	printf("C3 connected to server");
	//reading ip address
	read(cl_fd,msg,sizeof(msg));
	if (strcmp(ip,msg)==0)
	{
		printf("\nSending MAC to server %s",mac);
		write(cl_fd,mac,sizeof(mac));
	}
	else
	{
		printf("IP not matching");
	}
	
	return 0;
}

