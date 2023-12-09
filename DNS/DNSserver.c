#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct DNS {
  char ip[256];
  char name[256];
};
struct DNS table[256];

char *check_return_ipaddress(char *buffer) {
  for (int i = 0; i < 3; i++) {
    if ((strcmp(buffer, table[i].name)) == 10) {
      printf("IP Address: %s\n", table[i].ip);
      return table[i].ip;
    }
  }
  return "Not Found";
}

int main(int argc, char *argv[]) {
  // Initialize DNS Table
  strcpy(table[0].name, "google.com");
  strcpy(table[0].ip, "142.89.78.66");

  strcpy(table[1].name, "gmail.com");
  strcpy(table[1].ip, "10.2.45.67");

  strcpy(table[2].name, "ssn.edu.in");
  strcpy(table[2].ip, "197.34.53.122");  
  for (int i = 0; i < 3; i++) {
    printf("\n\t Domain name: %s", table[i].name);
    printf("\n\t IP Address: %s", table[i].ip);
    printf("\n");
  }
  printf("\n");
  // Socket setup and receiving data
  int server_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len;
  char buffer[1024];
  char new_ip[1024];
  char domain[1024];

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
    exit(1);
  }

  int Port = atoi(argv[1]);

  server_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_socket == -1) {
    perror("Socket Creation Failed!");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(Port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&server_addr,sizeof(server_addr)) == -1) {
    perror("Socket Bind Failed!");
    exit(1);
  }

  printf("Server Listening on Port %d .....\n", Port);

  int n;
  client_len = sizeof(client_addr);
  n = recvfrom(server_socket, buffer, sizeof(buffer), 0,(struct sockaddr *)&client_addr, &client_len);

  if (n == -1) {
    perror("Receive Failed");
    exit(1);
  } else {
    buffer[n] = '\0'; // Null-terminate the received data
    printf("Received message: %s\n", buffer);

    if (strcmp(buffer, "modify") == 0) {
      printf("Modifying..\n");

      int len = recvfrom(server_socket, domain, sizeof(domain), 0,(struct sockaddr *)&client_addr, &client_len);
      if (len == -1) {
        perror("Receive Failed");
        exit(1);
      }
      domain[len] = '\0'; // Null-terminate the received domain

      int m = recvfrom(server_socket, new_ip, sizeof(new_ip), 0,(struct sockaddr *)&client_addr, &client_len);
      if (m == -1) {
        perror("Receive Failed");
        exit(1);
      }
      new_ip[m] = '\0'; // Null-terminate the received IP address

      printf("Domain Received: %s\n", domain);
      printf("IP Received: %s", new_ip);

      for (int i = 0; i < 3; i++) {
        if (strcmp(domain, table[i].name) == 10) {
          strcpy(table[i].ip, new_ip);
          printf("IP Address updated for %s\n", domain);
          break;
        }
      }

      for (int i = 0; i < 3; i++) {
        printf("\n\t Domain name: %s", table[i].name);
        printf("\n\t IP Address: %s", table[i].ip);
        printf("\n");
      }
      printf("\n");

    } 
    
    else {
      char *ip_address = check_return_ipaddress(buffer);

      sendto(server_socket, ip_address, strlen(ip_address), 0,(struct sockaddr *)&client_addr, client_len);
      printf("Response sent to client\n");
    }
  }

  close(server_socket);
  return 0;
}



/*
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

struct DNS
{
    char name[255];
    char ip[255];
};
struct DNS table[256];

int main(int argc,char *argv[])
{
    strcpy(table[0].name,"google.com");
    strcpy(table[0].ip,"12.0.0.1");

    strcpy(table[1].name,"youtube.com");
    strcpy(table[1].ip,"14.0.0.1");

    strcpy(table[2].name,"gmail.com");
    strcpy(table[2].ip,"16.0.0.1");

    for(int i=0;i<3;i++)
    {
        printf("\n\t%s\t%s",table[i].name,table[i].ip);
    }
    printf("\n");

    int socketid=socket(AF_INET,SOCK_DGRAM,0);

    struct sockaddr_in servAddr,clientAddr;

    servAddr.sin_family=AF_INET;
    servAddr.sin_port=htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr=INADDR_ANY;

    socklen_t len=sizeof(clientAddr);

    bind(socketid,(struct sockaddr*)&servAddr,sizeof(servAddr));

    char buffer[255];
    char domain[255];
    char ip_addr[255];

    int n1=recvfrom(socketid,buffer,sizeof(buffer),0,(struct sockaddr*)&clientAddr,&len);
    buffer[n1]='\0';

    if(strcmp(buffer,"Modify")==0)
    {
        int n2=recvfrom(socketid,domain,sizeof(domain),0,(struct sockaddr*)&clientAddr,&len);
        domain[n2]='\0';

        int n3=recvfrom(socketid,ip_addr,sizeof(ip_addr),0,(struct sockaddr*)&clientAddr,&len);
        ip_addr[n3]='\0';
        
        for(int i=0;i<3;i++)
        {
            if(strcmp(table[i].name,domain)==0)
            {
                strcpy(table[i].ip,ip_addr);
                break;
            }
        }

        printf("\nUpdated....\n");

        for(int i=0;i<3;i++)
        {
            printf("\n\t%s\t%s",table[i].name,table[i].ip);
        }
        printf("\n");
    }
    else
    {
        int found=0;
        for(int i=0;i<3;i++)
        {
            if(strcmp(table[i].name,buffer)==0)
            {
                strcpy(ip_addr,table[i].ip);
                found=1;
                break;
            }
        }

        if(found==0)
        {
            char NotFound[255]="Not Found";
            sendto(socketid,NotFound,sizeof(NotFound),0,(struct sockaddr*)&clientAddr,len);
        }
        else
        {
            sendto(socketid,ip_addr,sizeof(ip_addr),0,(struct sockaddr*)&clientAddr,len);
        }
        printf("\nMessage sent from server\n");
    }
    close(socketid);

    return 0;

}

*/