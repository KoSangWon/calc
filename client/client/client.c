#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <sys/types.h>

#include <sys/socket.h>

 

#define BUFF_SIZE 1024

 

typedef struct {

    int num1;
    char op;
    int num2;

} SEND_PROTOCAL;

 

typedef struct {

    int result;

} RECV_PROTOCAL;

 

void msg_snd_rcv(int client_sock);

int main(int argc, char **argv)

{

    int client_socket;

    struct sockaddr_in server_addr;

 

    client_socket = socket(PF_INET,SOCK_STREAM,0);

    if(-1==client_socket)

    {

        printf("socket making is failed\n");

        exit(1);

    }

 

    memset(&server_addr,0x00,sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(4000);

    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);

   

    if(-1==connect(client_socket, (struct sockaddr*)&server_addr,sizeof(server_addr)))

    {

        printf("connect failed\n");

        exit(1);

    }

   

    printf("connected to server[%d]!\r\n", client_socket);

    msg_snd_rcv(client_socket);

}

 

void msg_snd_rcv(int client_sock)

{

    int len = 0;

    int mess_sock = client_sock;

//    char addtion[2];


    SEND_PROTOCAL send_proto;

    RECV_PROTOCAL recv_proto;

    while(1)

    {
        printf("입력 예시) 2 + 3\n");
        scanf("%d %c %d", &send_proto.num1, &send_proto.op, &send_proto.num2);
        write(mess_sock, &send_proto, sizeof(SEND_PROTOCAL));
        len = sizeof(recv_proto);
        read(mess_sock, &recv_proto, len);
        printf("%d\n", recv_proto.result);
}

}
