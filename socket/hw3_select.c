#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include "select.h"

int TcpSockfd; 
int UdpSockfd;

void CloseServer() // 서버 닫는 함수 
{
	close(TcpSockfd); // TCP 소켓 닫기  
	close(UdpSockfd); // UDP 소켓 닫기 

	printf("\nServer daemon exit.....\n");

	exit(0);
}

void MakeTcpSocket() // TCP 소켓 만들기 
{
	struct sockaddr_in servAddr;

	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // 0으로 초기화 
	servAddr.sin_family = PF_INET; // 프로토콜 지정 
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP
	servAddr.sin_port = htons(SERV_TCP_PORT); // Port

	if (bind(TcpSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) { // 소켓 주소 지정 
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5); // 대기 
}

void MakeUdpSocket() // UDP 소켓 만들기 
{
	struct sockaddr_in servAddr;

	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // 0으로 초기화 
	servAddr.sin_family = PF_INET; // 프로토콜 지정 
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP
	servAddr.sin_port = htons(SERV_UDP_PORT); // Port

	if (bind(UdpSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) { // 소켓 주소 지정 
		perror("bind");
		exit(1);
	}
}

void ThreadTcpRequest() // TCP 요청 쓰레드 
{
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr;
	MsgType msg;

	cliAddrLen = sizeof(cliAddr); 
	newSockfd = accept(TcpSockfd, (struct sockaddr *)&cliAddr, &cliAddrLen); // 소켓 연결 기다리기  
	if (newSockfd < 0) {
		perror("accept");
		exit(1);
	}

	if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) { // 메시지 읽기 
		perror("read");
		exit(1);
	}
	printf("Received TCP request: %s.....", msg.data); 

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %ld.", pthread_self()); // 쓰레드ID 저장 
	if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) { // 메시지 쓰기 
		perror("write");
		exit(1);
	}
	printf("Replied.\n");

	close(newSockfd); // 소켓 닫기 

	pthread_exit(NULL); // 쓰레드 종료 
}

void ThreadUdpRequest() // UDP 요청 쓰레드 
{
	int cliAddrLen, n;
	struct sockaddr_in cliAddr;
	MsgType msg;

	cliAddrLen = sizeof(cliAddr);
	if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0) { // 소켓에서 메시지 읽기 
		perror("recvfrom");
		exit(1);
	}
	printf("Received UDP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %ld.", pthread_self());
	if (sendto(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0) { // 소켓에서 메시지 쓰기 
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");

	pthread_exit(NULL); // 쓰레드 종료 
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	fd_set fdvar;
	int count;

	signal(SIGINT, CloseServer); // Cntl+C 시그널이 오면 서버 닫기 

	MakeTcpSocket(); // TCP 소켓 만들기 
	MakeUdpSocket(); // UDP 소켓 만들기 

	printf("Server daemon started.....\n");

	while (1) {
		FD_ZERO(&fdvar); // 파일 디스크립터 초기화 
		FD_SET(TcpSockfd, &fdvar); // 1로 설정 
		FD_SET(UdpSockfd, &fdvar); // 1로 설정 
		if ((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL)) < 0) { // 다수의 소켓을 처리 
			perror("select");
			exit(1);
		}
		while (count--) { // 파일 디스크립터 개수만큼 반복 
			if (FD_ISSET(TcpSockfd, &fdvar)) { // FD 이벤트 발생여부 체크 
				if (pthread_create(&tid, NULL, (void *)ThreadTcpRequest, (void *)NULL) < 0) { // 쓰레드 생성 
					perror("pthread_create");
					exit(1);
				}
			}
			else if (FD_ISSET(UdpSockfd, &fdvar)) { // FD 이벤트 발생여부 체크 
				if (pthread_create(&tid, NULL, (void *)ThreadUdpRequest, (void *)NULL) < 0) { // 쓰레드 생성 
					perror("pthread_create");
					exit(1);
				}
			}
		}
	}
}
