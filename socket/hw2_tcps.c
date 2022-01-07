#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include "tcp.h"

int Sockfd;

void CloseServer()
{
	close(Sockfd); // 소켓 제거 
	printf("\nTCP Server exit.....\n");

	exit(0);
}

int main(int argc, char *argv[])
{
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;
	pid_t pid;

	signal(SIGINT, CloseServer); // Ctrl+C 시그널 들어오면 소켓 제거 

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // servAddr 크기 바이트 만큼 0으로 초기화 
	servAddr.sin_family = PF_INET; // 프로토콜 지정 
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP
	servAddr.sin_port = htons(SERV_TCP_PORT); // Port

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) { // 소켓 주소 연결 
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); // 대기 

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1) {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); // 클라이언트 연결 
		if (newSockfd < 0) {
			perror("accept");
			exit(1);
		}

		if ((pid = fork()) < 0) { // 프로세스 생성 
			perror("fork");
			exit(1);
		}
		else if (pid == 0) { // Child Process
			if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) { // 메시지 읽기 
				perror("read");
				exit(1);
			}
			printf("Received request: %s.....", msg.data);

			msg.type = MSG_REPLY;
			sprintf(msg.data, "This is a reply from %d.", getpid());
			if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) { // 메시지 쓰기 
				perror("write");
				exit(1);
			}
			printf("Replied.\n");

			usleep(10000);
			close(newSockfd); // 소켓 종료 

			exit(0);
		}
		//parent 
		close(newSockfd); // 소켓 종료 
	}
}
