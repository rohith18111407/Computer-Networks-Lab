#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int client_sockets[4];

void *handle_client(void *cid) {
    int client_socket = *(int *)cid;
    char message[200];
    char reply[200];
    int count;
    for(int i =0;i<4;i++)
    {
    if (client_socket == client_sockets[i])
  count = i+1;
    }
    while (1) {
        read(client_socket, message, sizeof(message));
        printf("Client %d: %s\n",count, message);
        printf("Message: ");
        scanf("%[^\n]", reply);
        getchar();
        if (strcmp(reply, "bye") == 0)
            break;
        write(client_socket, reply, sizeof(reply));
        memset(message, '\0', sizeof(message));
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket >= 0) {
        struct sockaddr_in server_add, client_add;
        server_add.sin_family = AF_INET;
        server_add.sin_port = htons(atoi(argv[1]));
        server_add.sin_addr.s_addr = INADDR_ANY;
        bind(server_socket, (struct sockaddr *)&server_add, sizeof(server_add));
        if (listen(server_socket, 4) == -1) {
            printf("");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        pthread_t threads[4];
       
        int num_clients = 4;

        for (int i = 0; i < num_clients; i++) {
            client_sockets[i] = accept(server_socket, NULL, NULL);
            printf("Client %d has joined the chat now\n",i+1);
            if (client_sockets[i] < 0) {
                perror("");
            } else {
                pthread_create(&threads[i], NULL, handle_client, &client_sockets[i]);
            }
        }

        for (int i = 0; i < num_clients; i++) {
            pthread_join(threads[i], NULL);
        }

        close(server_socket);
    }

    return 0;
}

/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

int clientsocket[4];

void *handle_client(void *cid)
{
    int clientsockid=*(int*)cid;
    char msg[100];
    int count=0;
    for(int i=0;i<4;i++)
    {
        if(clientsockid==clientsocket[i])
        {
            count=i+1;
        }
    }

    while(1)
    {
        recv(clientsockid,msg,sizeof(msg),0);
        printf("Cleint %d: %s",count,msg);

        printf("Enter message: ");
        fgets(msg,sizeof(msg),stdin);
        if(strncmp(msg,"end",3)==0)
        {
            break;
        }
        send(clientsockid,msg,sizeof(msg),0);
        memset(msg,'\0',sizeof(msg));
    }
    close(clientsockid);
    pthread_exit(NULL);

}

int main(int argc,char *argv[])
{
    int serversocketid=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in servAddr,clientAddr;

    servAddr.sin_family=AF_INET;
    servAddr.sin_port=htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr=INADDR_ANY;

    bind(serversocketid,(struct sockaddr*)&servAddr,sizeof(servAddr));
    listen(serversocketid,4);

    int num_clients=4;
    pthread_t threads[4];

    for(int i=0;i<num_clients;i++)
    {
        clientsocket[i]=accept(serversocketid,NULL,NULL);
        printf("\nClient %d joined the chat...\n",i+1);
        pthread_create(&threads[i],NULL,handle_client,&clientsocket[i]);
    }

    for(int i=0;i<num_clients;i++)
    {
        pthread_join(threads[i],NULL);
    }

    close(serversocketid);

    return 0;
}


*/