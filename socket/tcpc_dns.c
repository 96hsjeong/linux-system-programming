#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include "tcp.h"

int main(int argc, char *argv[])
{
	int sockfd, n;
	struct sockaddr_in servAddr;
	MsgType msg;
	struct hostent *hp;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // 0으로 초기화 
	servAddr.sin_family = PF_INET; // 프로토콜 지정 
	servAddr.sin_port = htons(SERV_TCP_PORT); // Port 

	if (isdigit(argv[1][0])) { // IP 주소가 숫자로 들어오는 경우 
		servAddr.sin_addr.s_addr = inet_addr(argv[1]); // IP
	}
	else { // 도메인 네임으로 들어오는 경우 
		if ((hp = gethostbyname(argv[1])) == NULL) { // 호스트 네임에 상응하는 구조체로 반환 
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length); // 바이트 복사 
	}

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

	close(sockfd); // 소켓 종료 
}



