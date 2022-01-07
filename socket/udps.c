#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include "udp.h"

int Sockfd;

void CloseServer()
{
	close(Sockfd); // 소켓 종료 
	printf("\nUDP Server exit.....\n");

	exit(0);
}

int main(int argc, char *argv[])
{
	int cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); 
	servAddr.sin_family = PF_INET; // 프로토콜 지정 IPv4
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP
	servAddr.sin_port = htons(SERV_UDP_PORT); // Port 

	if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) { // 소켓 주소 지정 
		perror("bind");
		exit(1);
	}

	printf("UDP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1) {
		if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0) { // 메시지 읽기 
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY; // 메시지 응답
		sprintf(msg.data, "This is a reply from %d.", getpid()); 
		if (sendto(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0) { // 메시지 쓰기 
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}

