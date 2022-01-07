#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "tcp.h"

int main(int argc, char *argv[])
{
	int sockfd, n;
	struct sockaddr_in servAddr;
	MsgType msg;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // 0으로 초기화 
	servAddr.sin_family = PF_INET; // 프로토콜 지정 
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // IP
	servAddr.sin_port = htons(SERV_TCP_PORT); // PORT 

	if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) { // 서버 연결 
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST; // 메시지 요청 
	sprintf(msg.data, "This is a request from %d.", getpid()); // 메시지 데이터 저장 
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0) { // 메시지 쓰기  
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");

	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0) { // 메시지 읽기 
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);
}
