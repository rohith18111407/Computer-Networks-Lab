/*
Run sarp first and then all other carps
pass port number in sarp
pass port number ip address, mac address in carps
only valid ip address's mac address will be sent to server
*/


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
	int ser_fd,cl1,cl2;
	int clients[3];
	struct sockaddr_in ser_add,cl1_add,cl2_add,cl3_add;
	char ip[256];
	char reply[256];
	
	
	ser_add.sin_family=AF_INET;
	ser_add.sin_addr.s_addr=INADDR_ANY;
	ser_add.sin_port=htons(atoi(argv[1]));
	
	if ((ser_fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("\nSocket Failure");
		exit(1);
	}
	
	if ((bind(ser_fd,(struct sockaddr*) &ser_add,sizeof(ser_add)))<0)
	{
		perror("\nBinding Error");
		exit(1);
	}
	
	if ((listen(ser_fd,10))<0)
	{
		perror("\nError");
		exit(1);
	}
	
	//cl1=accept(ser_fd,NULL,NULL)
	/*c1=sizeof(cl1_add);
	cl1=accept(ser_fd,(struct sockaddr*)&cl1_add,&c1);
	
	int c2=sizeof(cl2_add);
	cl2=accept(ser_fd,(struct sockaddr*)&cl2_add,&c2);
	
	int c3=sizeof(cl3_add);
	cl3=accept(ser_fd,(struct sockaddr*)&cl3_add,&c3);*/
	
	for (int i=0;i<3;i++)
	{
		clients[i]=accept(ser_fd,NULL,NULL);
	}
	
	//getting ip address and data
	printf("Enter IP: ");
	scanf("%s",ip);
	
	
	/*write(cl1,ip,sizeof(ip));
	write(cl2,ip,sizeof(ip));
	write(cl3,ip,sizeof(ip));*/
	for (int i=0;i<3;i++)
	{
		write(clients[i],ip,sizeof(ip));
	}
	
	for (int i=0;i<3;i++)
	{
		read(clients[i],reply,sizeof(reply));
		printf("%s\n",reply);
		memset(reply,'\0',sizeof(reply));
	}
	
	return 0;
}
