#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char message[200];
    char new[200];
    printf("Enter message: ");
    scanf("%[^\n]", message);
    getchar();
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(atoi(argv[1]));
    server_add.sin_addr.s_addr = inet_addr(argv[2]);
    if (client_socket)
    {
        if (connect(client_socket, (struct sockaddr *)&server_add, sizeof(server_add)) == 0)
        {
            write(client_socket, message, strlen(message));

            while (1)
            {
                read(client_socket, new, sizeof(new));
                printf("Server: %s\n", new);
                printf("Type message: ");
                scanf("%[^\n]", message);
                getchar();
                if (strcmp(message, "bye") == 0)
                    break;
                write(client_socket, message, strlen(message));
            }
        }
        else
        {
            perror("");
        }
    }
    else
    {
        perror("");
    }
    return 0;
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    int socketid=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in clientAddr;

    memset(&clientAddr,0,sizeof(clientAddr));

    clientAddr.sin_family=AF_INET;
    clientAddr.sin_port=htons(atoi(argv[1]));
    clientAddr.sin_addr.s_addr=inet_addr(argv[2]);

    int connectstatus=connect(socketid,(struct sockaddr*)&clientAddr,sizeof(clientAddr));

    char msg[100];

    while(1)
    {
        printf("\nEnter message to server: ");
        fgets(msg,sizeof(msg),stdin);

        if(strncmp(msg,"end",3)==0)
        {
            break;
        }

        send(socketid,msg,sizeof(msg),0);

        recv(socketid,msg,sizeof(msg),0);

        printf("\nserver: %s",msg);
    }
    close(socketid);

    return 0;
}
*/