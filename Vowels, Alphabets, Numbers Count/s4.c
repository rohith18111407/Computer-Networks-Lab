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

	int sockD = socket(AF_INET, SOCK_STREAM, 0);
	char echo[50];

	int counts[] = {0,0,0,0};
	int i;
	char c;

	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port= htons(9001); // use some unused port number
	servAddr.sin_addr.s_addr = INADDR_ANY;

	int connectStatus= connect(sockD, (struct sockaddr*)&servAddr,sizeof(servAddr));

	if (connectStatus == -1) {
		printf("Error...\n");
	}

	else {
		char strData[255];

		recv(sockD, strData, sizeof(strData), 0);

		printf("Message received from client: \n%s\n", strData);
		
		for(i=0;strData[i]!='\0';i++){
			c = strData[i];
			
			if(c=='a' || c=='e' || c=='i' || c=='o' || c=='u'){
				counts[1]++;
				counts[0]++;
			}
			else if(c=='A' || c=='E' || c=='I' || c=='O' || c=='U'){
				counts[1]++;
				counts[0]++;
			}
			else if((c>='a' && c<='z') || (c>='A' && c<='Z')){
				counts[0]++;
			}
			else if(c>='1' && c<='9'){
				counts[2]++;
			}
			else{
				counts[3]++;
			}
			
		}
		
		//send(sockD,counts,sizeof(counts),0);
		send(sockD,&counts[0],sizeof(counts[0]),0);
		send(sockD,&counts[1],sizeof(counts[1]),0);
		send(sockD,&counts[2],sizeof(counts[2]),0);
		send(sockD,&counts[3],sizeof(counts[3]),0);
	}

	return 0;
}

