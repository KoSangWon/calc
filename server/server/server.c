#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <pthread.h>

 

#define BUFF_SIZE 1024

#define MAX_CLIENT 256

 

typedef struct {

    int num1;
    char op;
    int num2;

} RECV_PROTOCAL;

 

typedef struct {

    int result;

} SEND_PROTOCAL;

 

pthread_mutex_t mutx;

void *addition_calculator(void * arg);

int calculator(int in_data1, int in_data2);

 

int main(int argc, char **argv)

{

    int sock_fd=0;

    int client_sock_fd=0;

    unsigned int client_size=0;

   

    struct sockaddr_in server_addr, client_addr;

 

    pthread_t p_id;

   

    memset(&server_addr,0x00,sizeof(server_addr));

    memset(&client_addr,0x00,sizeof(client_addr));

   

    sock_fd=socket(PF_INET,SOCK_STREAM,0);

    if(-1==sock_fd)

    {

        printf("server socket making is failed!\n");

        exit(1);

    }

 

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(4000);

    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);

   

    if(-1==bind(sock_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))

    {

        printf("bind() failed!\n");

        exit(1);

    }

 

    if(-1==listen(sock_fd,5))

    {

        printf("listen() failed\n");

        exit(1);

    }

 

    while(1)

    {

        client_size = sizeof(client_addr);
        client_sock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_size);
        pthread_create(&p_id, NULL, addition_calculator, (void*)&client_sock_fd);
        pthread_detach(p_id);

    }

}

 

void *addition_calculator(void *arg)

{

    int clnt_sock;

    int str_len = 0;

    int cal_data = 0;

   

    SEND_PROTOCAL send_proto;

    RECV_PROTOCAL recv_proto;

   

    memset(&send_proto, 0x00, sizeof(SEND_PROTOCAL));

    memset(&recv_proto, 0x00, sizeof(RECV_PROTOCAL));

   

    clnt_sock = *((int *)arg);

   

    while(1)

    {

        if(read(clnt_sock, &recv_proto, sizeof(RECV_PROTOCAL)) < 0){
            printf("failed\n");
            break;
        }
        printf("%d + %d", recv_proto.num1, recv_proto.num2);
        cal_data = calculator(recv_proto.num1, recv_proto.num2);
        str_len = sizeof(cal_data);
        write(clnt_sock, &cal_data, str_len);
}

    close(clnt_sock);

    return NULL;
}

 


int calculator(int in_data1, int in_data2)

{

   int sum = 0;

 
    pthread_mutex_lock(&mutx);
    sum = in_data1+in_data2;
    pthread_mutex_unlock(&mutx);

 

    return (sum);

}

 
