#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"

#define	DEBUG

#define	MAX_CLIENT	5
#define	MAX_ID		32
#define	MAX_BUF		256

// 구조체 정의 
typedef	struct  {
	int	sockfd; // 소켓 
	int	inUse; 
	pthread_t tid; // 쓰레드 ID
	char uid[MAX_ID]; // 사용자 ID
} ClientType;

int	Sockfd;
pthread_mutex_t	Mutex;

ClientType Client[MAX_CLIENT];

int GetID() { // i번째 사용자 
	int	i;

	for (i = 0 ; i < MAX_CLIENT ; i++)  { // MAX_CLIENT 만큼 반복 
		if (! Client[i].inUse) { // 사용 중이 아닌 번호 
			Client[i].inUse = 1; // 사용 중으로 변경 
			return i; // 번호 반환 
		}
	}
}

void SendToOtherClients(int id, char *buf) // 메시지 보내기 
{
	int i;
	char msg[MAX_BUF+MAX_ID];

	sprintf(msg, "%s> %s", Client[id].uid, buf); // 메시지 저장 
#ifdef	DEBUG
	printf("%s", msg); // 메시지 출력 
	fflush(stdout); // 비우기 
#endif
	pthread_mutex_lock(&Mutex); // mutex lock
	for (i = 0 ; i < MAX_CLIENT ; i++)  { // MAX_CLIENT 만큼 반복 
		if (Client[i].inUse && (i != id))  { // 사용 중이고 본인이 아닌 사람에게 
			if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  { // 메시지 보내기 
				perror("send");
				exit(1);
			}
		}
	}
	pthread_mutex_unlock(&Mutex); // mutex unlock 
}
	

void ProcessClient(int id) { // Client 프로세스 
	char buf[MAX_BUF];
	int n;

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  { 
		perror("pthread_setcancelstate");
		exit(1);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		exit(1);
	}

	if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0)  { // 소켓으로부터 수신 
		perror("recv");
		exit(1);
	}
	printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid); // 연결된 사용자 정보 출력 

	while (1)  {
		if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0)  { // 소켓으로부터 수신 
			perror("recv");
			exit(1);
		}
		if (n == 0)  { // 연결이 끊기면 
			printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid); // 로그아웃한 사용자 출력 

			pthread_mutex_lock(&Mutex); // mutex lock 
			close(Client[id].sockfd); // 소켓 종료 
			Client[id].inUse = 0; // 사용중 아님 
			pthread_mutex_unlock(&Mutex); // unlock 

			strcpy(buf, "log-out.....\n"); // 메시지 저장 
			SendToOtherClients(id, buf); // 메시지 보내기 

			pthread_exit(NULL); // 쓰레드 종료 
		}

		SendToOtherClients(id, buf); // 메시지 보내기 
	}
}


void CloseServer(int signo) { // 서버 닫기 
	int	i;

	close(Sockfd); // 소켓 닫기 

	for (i = 0 ; i < MAX_CLIENT ; i++)  { // MAX_CLIENT 만큼 반복 
		if (Client[i].inUse)  { // 사용 중인 경우 
			if (pthread_cancel(Client[i].tid))  { // 해당 쓰레드 종료 
				perror("pthread_cancel");
				exit(1);
			}
			if (pthread_join(Client[i].tid, NULL))  { // 해당 쓰레드 종료 대기 
				perror("pthread_join");
				exit(1);
			}
			close(Client[i].sockfd); // 소켓 닫기 
		}
	}
	if (pthread_mutex_destroy(&Mutex) < 0)  { // mutex 종료 
		perror("pthread_mutex_destroy");
		exit(1);
	}

	printf("\nChat server terminated.....\n");

	exit(0);
}

int main(int argc, char *argv[])
{
	int newSockfd, cliAddrLen, id, one = 1;
	struct sockaddr_in cliAddr, servAddr;

	signal(SIGINT, CloseServer); // Ctrl + C 시그널 받으면 서버 종료 
	if (pthread_mutex_init(&Mutex, NULL) < 0)  { // mutex 생성 
		perror("pthread_mutex_init");
		exit(1);
	}

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // 소켓 생성 
		perror("socket");
		exit(1);
	}

	if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)  { // 소켓 옵션 설정 
		perror("setsockopt");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET; // 프로토콜 지정 
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP
	servAddr.sin_port = htons(SERV_TCP_PORT); // PORT

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // 소켓 주소 생성 
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); // 대기 

	printf("Chat server started.....\n");

	cliAddrLen = sizeof(cliAddr); 
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); // 소켓 연결되면 
		if (newSockfd < 0)  { 
			perror("accept");
			exit(1);
		}

		id = GetID();
		Client[id].sockfd = newSockfd; 
		
		if (pthread_create(&Client[id].tid, NULL, (void *)ProcessClient, (void *)id) < 0)  { // 쓰레드 생성 
			perror("pthread_create");
			exit(1);
		}
	}
}
