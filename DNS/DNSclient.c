#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <Server_IP> <Port>\n", argv[0]);
    exit(1);
  }

  int Port = atoi(argv[2]);
  char buffer[MAX];
  char new_ip[MAX];
  char domain[MAX];
  const char *request = "modify";

  int sockfd;
  struct sockaddr_in server_addr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(1);
  }

  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(Port);
  server_addr.sin_addr.s_addr = inet_addr(argv[1]); // Use provided server IP

  int choice;
  printf("Enter 1 to get IP for Domain Name\nEnter 2 to change IP Address\n");
  int s = scanf("%d", &choice);
  getchar();

  if (choice == 1) {
    printf("Provide Domain Name : ");
    char *f = fgets(buffer, MAX, stdin);
    sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Message Sent From Client\n");

    char ip_buffer[MAX];
    socklen_t len = sizeof(server_addr);
    int n = recvfrom(sockfd, ip_buffer, sizeof(ip_buffer), 0,(struct sockaddr *)&server_addr, &len);
    ip_buffer[n] = '\0';
    printf("Received IP Address from Server: %s\n", ip_buffer);
  } 
  
  else if (choice == 2) {
    sendto(sockfd, request, strlen(request), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Message Sent From Client\n");

    printf("Provide Domain : ");
    char * g = fgets(domain,MAX,stdin);
    sendto(sockfd, domain, strlen(domain), 0,(const struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Enter new IP : ");
    char *f = fgets(new_ip, MAX, stdin);
    sendto(sockfd, new_ip, strlen(new_ip), 0,(const struct sockaddr *)&server_addr, sizeof(server_addr));
    
    printf("Message Sent From Client\n");
  }
  
   else {
    printf("Invalid choice. Exiting.\n");
  }

}


/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char *argv[])
{
    int socketid=socket(AF_INET,SOCK_DGRAM,0);

    struct sockaddr_in clientaddr;

    clientaddr.sin_family=AF_INET;
    clientaddr.sin_port=htons(atoi(argv[1]));
    clientaddr.sin_addr.s_addr=inet_addr(argv[2]);

    char buffer[255];
    char domain[255];
    char new_ip[255];
    char request[255]="Modify";
    int choice;

    printf("\nChoose an option \n1.Modify IP address \n2.Receive IP address\n ");
    scanf("%d",&choice);

    if(choice==1)
    {
        sendto(socketid,request,sizeof(request),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));

        printf("\nEnter domain name: ");
        scanf("%s",domain);

        sendto(socketid,domain,sizeof(domain),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));

        printf("\nEnter new ip: ");
        scanf("%s",new_ip);

        sendto(socketid,new_ip,sizeof(new_ip),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));

        printf("\nMessage sent from client to server\n");
    }

    else if(choice==2)
    {
        printf("\nEnter the domain name: ");
        scanf("%s",domain);

        sendto(socketid,domain,sizeof(domain),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));

        socklen_t len=sizeof(clientaddr);

        printf("\nMessage sent from client to server\n");

        int n=recvfrom(socketid,new_ip,sizeof(new_ip),0,(struct sockaddr*)&clientaddr,&len);
        new_ip[n]='\0';

        printf("\nIP address: %s",new_ip);
    }

    else
    {
        printf("\nInvalid choice....");
    }
    close(socketid);

    return 0;
}
*/