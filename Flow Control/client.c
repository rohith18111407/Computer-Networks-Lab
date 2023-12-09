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
//#define IPADDR argv[1]
#define IPADDR "127.0.0.1"
//#define PORT atoi(argv[2])


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
	int len,sock;
	bzero((char *)&sin, sizeof(sin));
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(IPADDR);
	sin.sin_port = htons(PORT);

	sock = socket(PF_INET, SOCK_STREAM, 0); 
	check(sock);

	check(connect(sock, (struct sockaddr *)&sin, sizeof(sin)));
	printf("Connected to server\n");

	char buffer[1024];
	char data[100];
	char ch = 'N';
	
	while(ch!='Y')
	{
		printf("Data :");
		fgets(data,sizeof(data),stdin);
		data[strcspn(data,"\n")] = '\0';
		
		char d,z;
		printf("Enter source address:");
		d = fgetc(stdin);
		fgetc(stdin);
		printf("Enter dest address:");
		z = fgetc(stdin);
		fgetc(stdin);
		
		int no = ceil(((double)strlen(data))/4);

		printf("Size = %d so %d Packets\n",strlen(data),no);		
		send(sock,"START",strlen("START")+1,0);
		
		char dummy[5];
		int ss = 0,c=0;
		
		for(ss = 0;ss<strlen(data);ss+=4)
		{
			strncpy(dummy,data+ss,4);
			sprintf(buffer,"F-%c-%d-%s -FCS-F",z,c,dummy);

			printf("Send(Y/N):");
			ch = fgetc(stdin);
			fgetc(stdin);
			
			if(ch == 'Y'){
				int r = send(sock,buffer,strlen(buffer)+1,0);
				printf("\tSent  :%s\n",buffer);
			}
			else
			{
				printf("\tNot Sent :%s\n",buffer);
			}
			c++;
			printf("Sent packet no:%d\n",c);
		}
		
		send(sock,"STOP",strlen("STOP")+1,0);

		printf("Sent \"STOP\" expecting response as ACK%d ",c);

		len = recv(sock,buffer,sizeof(buffer),0);
		int ack;
		sscanf(buffer,"ACK%d",&ack);	//It is used to read input from a string, a buffer (temporary storage that stores input/output commands), or a character array.
		if(ack == c)
		{
			send(sock,"TSUCCESS",strlen("TSUCCESS")+1,0);
			printf("-Got the response\n");
		}
		
		else
		{
			printf("-But got \"ACK%d\"\n",ack);
			printf("Sending missing packets\n");
			c = ack;
			for(ss = ack*4;ss<strlen(data);ss+=4)
			{
				strncpy(dummy,data+ss,4);
				sprintf(buffer,"F-%c-%d-%s -FCS-F",z,c,dummy);
				send(sock,buffer,strlen(buffer)+1,0);
				printf("\t~Sent :%s\n",buffer);

				c++;
				printf("\t~Sent packet no:%d\n",c);
			}
			send(sock,"TSUCCESS",strlen("TSUCCESS")+1,0);
			
		}
		printf("Sent Transmission Success\n");

		printf("End Transmission (Y/N):");
		ch = fgetc(stdin);
		fgetc(stdin);
	}
	send(sock,"EXIT",strlen("EXIT")+1,0);
	close(sock);

} 
