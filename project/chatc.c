#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include "chat.h"

#define	MAX_BUF	256

int Sockfd;

void ChatClient(void) { // 채팅 로그인 
	char buf[MAX_BUF];
	int	count, n;
	fd_set fdset;

	printf("Enter ID: "); // ID 입력 
	fflush(stdout);
	fgets(buf, MAX_BUF, stdin); // ID
	*strchr(buf, '\n') = '\0';
	if (send(Sockfd, buf, strlen(buf)+1, 0) < 0)  { // 소켓 보내기 
		perror("send");
		exit(1);
	}
	printf("Press ^C to exit\n");

	while (1)  { // 중지될때까지 반복 
		FD_ZERO(&fdset); 
		FD_SET(Sockfd, &fdset);
		FD_SET(STDIN_FILENO, &fdset);

		if ((count = select(10, &fdset, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL)) < 0)  { // 파일스크립터 변화 확인 
			perror("select");
			exit(1);
		}
		while (count--)  { 
			if (FD_ISSET(Sockfd, &fdset))  {
				if ((n = recv(Sockfd, buf, MAX_BUF, 0)) < 0)  { // 
					perror("recv");
					exit(1);
				}
				if (n == 0)  { // 서버가 종료되면
					fprintf(stderr, "Server terminated.....\n");
					close(Sockfd); // 소켓 닫기 
					exit(1);
				}
				printf("%s", buf);
			}
			else if (FD_ISSET(STDIN_FILENO, &fdset))  {
				fgets(buf, MAX_BUF, stdin); // 메시지 저장 
				if ((n = send(Sockfd, buf, strlen(buf)+1, 0)) < 0)  { // 메시지 전송 
					perror("send");
					exit(1);
				}
			}
		}
	}
}
	

void CloseClient(int signo) { // 로그아웃 
	close(Sockfd); // 소켓 닫기 
	printf("\nChat client terminated.....\n");

	exit(0);
}


int main(int argc, char *argv[]) {
	struct sockaddr_in servAddr;
	struct hostent *hp;

	if (argc != 2)  { // 필요한 만큼 인자가 없으면 에러 출력 
		fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
		exit(1);
	}

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET; // 프로토콜 지정 
	servAddr.sin_port = htons(SERV_TCP_PORT); // PORT

	if (isdigit(argv[1][0]))  { // 숫자이면 
		servAddr.sin_addr.s_addr = inet_addr(argv[1]); // IP
	}
	else  { // 숫자가 아니면 
		if ((hp = gethostbyname(argv[1])) == NULL)  { // 도메인 네임을 IP로 
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length); // IP
	}

	if (connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // 소켓 연결 
		perror("connect");
		exit(1);
	}

	signal(SIGINT, CloseClient); // Ctrl + C 시그널 들어오면 로그아웃 

	ChatClient(); // 채팅 로그인 
}
