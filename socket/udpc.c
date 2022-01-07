#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "udp.h"

int main(int argc, char *argv[])
{
	int sockfd, n, peerAddrLen;
	struct sockaddr_in servAddr, peerAddr;
	MsgType msg;

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); 
	servAddr.sin_family = PF_INET; // 프로토콜 지정 
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // IP
	servAddr.sin_port = htons(SERV_UDP_PORT); // Port

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (sendto(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) { // 메시지 쓰기 
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");

	peerAddrLen = sizeof(peerAddr);
	if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0) { // 메시지 읽기 
		perror("recvfrom");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd); // 소켓 종료 
}
