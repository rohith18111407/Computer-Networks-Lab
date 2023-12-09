#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#define PORT atoi(argv[1])
#define PORT 2003 

void check(int ret)
{
	if(ret <0)
	{
		perror("ERROR!!!!!");
		exit(-1);
	}
}

int main()
{
	struct sockaddr_in sin;

	int len,s,ns;
	bzero((char *)&sin, sizeof(sin));
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(PORT);
	
	s = socket(PF_INET, SOCK_STREAM, 0); 
	check(s);
	
	len = bind(s,(struct sockaddr *)&sin,sizeof(sin)); 
	check(len);
	
	listen(s, 5);
	printf("Waiting for Client\n");
	
	ns = accept(s, (struct sockaddr *)&sin, &len); 
	check(ns);
	
	printf("Client Connected\n");
	char buffer[1024];
	char data[100];
	data[0] = '\0';
	
	int prev = -1;
	int count = 0;
	
	while(1)
	{
		len = recv(ns,buffer,sizeof(buffer),0);
		//printf("%s\n",buffer);
		if(strncmp(buffer,"EXIT",4)==0)
			break;
		
		else if(strncmp(buffer,"START",5)==0)
		{
			count = 0;
			prev = -1;
			data[0] = '\0';
			printf("[~Transmission Started~]\n");
		}
		else if(strncmp(buffer,"STOP",4)==0)
		{
			printf("Received \"STOP\"\n");
			sprintf(buffer,"ACK%d",count);
			send(ns,buffer,strlen(buffer)+1,0);
			printf("Sending acknowledgement as \"%s\"\n",buffer);
		}
		else if(strncmp(buffer,"TSUCCESS",8)==0)
		{
			printf("\nDATA = %s\n",data);
			printf("[~Transmission Ended~]\n\n");
		}
		else
		{
			int cur = 0;
			char d,dummy[10];
			
			sscanf(buffer,"F-%c-%d-%s -FCS-F",&d,&cur,dummy);
			printf("Received Message:%s\n",buffer);
			if(prev+1 == cur)
			{
				prev = cur;
				count++;
				strcat(data,dummy);
			}
			else
			{
				printf("Missing Packet %d\n",prev+1);			
			}
		}
		
	}
	printf("Server Exited\n");
	close(ns);
	close(s);

} 
