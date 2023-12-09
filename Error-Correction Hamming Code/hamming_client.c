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

int generateR(int databits){
    int r = 0;
    while(pow(2,r) <= databits + r + 1){
        r++;
    }
    return r;
}

message  *encode(char *buf,message *m) {
    int i, total,r,val,bit;
    int dpos = 0;
    char ch;
    r = m->rbits - 1;
    total = m->totalbits;

    int *tempArr = (int *)malloc(total * sizeof(int));
    
    for (i = total - 1; i >= 0; i--) { // Start from the index position 0 and move to the right
        if ((i + 1) == pow(2, r) && r>=0) {
            m->data[i] = 0; // Initialize parity bit positions to 0
            r--;
            //printf("arr[%d] = %d \n",i,arr[i]);
        } else {
            m->data[i] =  buf[dpos++] - '0';
            //printf("arr[%d] = %d buf[%d] = %c \n",i,arr[i],i,buf[i]);
        }
    }

    printf("Starting encoding\n");
    for(int i=0;i< total;i++){
        printf("Data[%d]= %d\n",i,m->data[i]);
    }

    memcpy(tempArr, m->data, total * sizeof(int));
    
    for (int i = 0; i < m->rbits; i++) {
        int position = 1 << i; // Calculate the position (1, 2, 4,8,...)
        //printf("pos:%d\n",position);
        m->data[position - 1] = calculateParityBit(tempArr,position,total);
        printf("R%d: %d\n",i,m->data[position-1]);
    }

    val = bin_to_dec(m->data,m->totalbits,m->rbits);
    printf("Decimal value of [r8 r4 r2 r1] = %d\n",val);

    printf("Want to change any bit position before sending!: ");
    scanf("%c",&ch);

    if(ch == 'y'){
        printf("Tell the bit position you want to change: ");
        scanf("%d",&bit);
        if(m->data[bit-1] == 0)
            m->data[bit - 1] = 1;
        else
            m->data[bit-1] = 0;
    }

    printf("Data after encoding:-\n");
    for(int i=total-1;i>=0 ;i--){
        printf("%d ",m->data[i]);
    }
    
	return m;
}


int main(int argc, char **argv){
	struct sockaddr_in sin;
	int len,s,red,size,total,val;
	char buffer[1024];

	message *ham;
	ham = (message *)malloc(sizeof(message));
	bzero((char *)&sin, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(argv[2]);
	sin.sin_port = htons(atoi(argv[1]));
	
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}

	printf("Enter the binary sequence: ");
	fgets(buffer,sizeof(buffer),stdin);

	ham->databits = strlen(buffer) - 1;
	printf("%d\n",ham->databits);

	ham->rbits = generateR(ham->databits);
	ham->totalbits = ham->rbits + ham->databits;
	ham->data = (int *)malloc(ham->totalbits * sizeof(int));

	for(int i=0;i< ham->totalbits;i++){
		ham->data[i] = 0;
	}
	ham = encode(buffer,ham);

	send(s,ham,sizeof(message),0);
	send(s, ham->data, sizeof(int) * ham->totalbits, 0);
    printf("Client has sent the message\n");
    close(s);
	
}

