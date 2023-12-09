#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <math.h>
#define SERVER_PORT 5436
#define MAX_PENDING 5
#define MAX_LINE 256

typedef struct message{
    int *data;
    int rbits;
    int totalbits;
    int databits;
}message;

int calculateParityBit(int *data, int position,int totalbits) {
    int parity = 0;
    for (int i = position-1; i < totalbits ; i += position * 2) {
        for (int j = i ; j < i + position && j < totalbits  ; j++) {
            parity ^= data[j]; // XOR operation for parity calculation
           //printf("arr[%d] = %d\n",j ,data[j]);
        }
    }
    return parity;
}

int bin_to_dec(int *data,int totalbits,int rbits){
    int pos,j,decimal = 0;
    for(j=0;j<rbits;j++){
        pos = 1<<j;
        //printf("pos :%d\n",pos);
        if (data[pos-1]==1){
            //printf("data[%d] = %d\n",pos-1,data[pos-1]);
            decimal+=pos;
        }
    }
    return decimal;
}

void decode(message *m){
    int i,val;
    int *tempArr = (int *)malloc(m->totalbits * sizeof(int));
    printf("Started decoding!\n");

    memcpy(tempArr, m->data, m->totalbits * sizeof(int));
    for (int i = 0; i < m->rbits; i++) {
        int position = 1 << i; // Calculate the position (1, 2, 4,8,...)
        //printf("pos:%d\n",position);
        tempArr[position - 1] = calculateParityBit(m->data,position,m->totalbits);
        printf("R%d: %d\n",i,tempArr[position-1]);
    }

    val = bin_to_dec(tempArr,m->totalbits,m->rbits);
    printf("Decimal value of [r8 r4 r2 r1] [Error position]= %d\n",val);
    
    if(val!=0){
        if(m->data[val-1] == 1)
            m->data[val-1] = 0;
        else
            m->data[val-1] = 1;
        printf("After correcting the %d th bit\n",val);
    }

    printf("Data received from client is found after decoding:-\n");
    
    for(int i=m->totalbits-1;i>=0 ;i--){
        printf("%d ",m->data[i]);
    }
}

int main(int argc, char **argv){
	struct sockaddr_in sin;

	int len,s,new_s;
	char buffer[MAX_LINE],ch;
	
    message *ham;
	ham = (message *)malloc(sizeof(message));
    ham->data = NULL;
	//char sentback[50];
	bzero((char *)&sin, sizeof(sin));
	
    sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(atoi(argv[1]));
	
	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
		perror("simplex-talk: bind");
		exit(1);
	}
	
    listen(s, MAX_PENDING);
	len = sizeof(sin);

	if ((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0) {
		perror("simplex-talk: accept");
		exit(1);
	}

	if((len = recv(new_s,ham,sizeof(message),0))<0){
		printf("Error\n");
        exit(1);
	}
    
    ham->data = (int *)malloc(sizeof(int) * ham->totalbits);
    
    if (recv(new_s, ham->data, sizeof(int) * ham->totalbits, 0) < 0) {
        perror("Error receiving data array");
        free(ham->data);
        free(ham);
        exit(1);
    }
	
    printf("Data received from client:-\n");
	printf("Total bits is %d\n",ham->totalbits);
    
    for (int i = 0; i < ham->totalbits; i++) {
        printf("arr[%d] = %d\n", i, ham->data[i]);
    }

    decode(ham); // Free the dynamically allocated memory
	
    free(ham->data);
    free(ham);
	close(new_s);
	close(s); 
	
}
