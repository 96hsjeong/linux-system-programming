// 숙제3: 추가 프로그램 작성
//   - 1. mutex와 condition variable을 사용 
// 1과 2의 방법을 비교: 어떤 방법을 사용하더라도 똑같이 구현할 수 있다.  

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include "semlib2.h" // mutex & condition variable 사용하여 세마포어 구현 

#define MAX_BUF 128

sem_t Sem1, Sem2;
char Data[MAX_BUF];

void Sipc1(void *dummy)
{
	printf("Receiver -> thread id = %ld\n", pthread_self()); // 쓰레드 아이디 출력 

	if (sem_wait(&Sem2) < 0) { // Sem2가 0이면 대기, 아니면 -1 
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("Receiver -> Received request: %s\n", Data); // Data 출력 
	sprintf(Data, "This is a reply from %ld.", pthread_self()); // 쓰레드 아이디 저장 

	if (sem_post(&Sem1) < 0) { // Sem1 +1;
		perror("sem_post");
		pthread_exit(NULL);
	}

	printf("Receiver -> Replied.\n");

	pthread_exit(NULL); // 쓰레드 종료 
}


void Sipc2(void *dummy)
{
	printf("Sender   -> thread id = %ld\n", pthread_self()); // 쓰레드 아이디 출력 

	if (sem_wait(&Sem1) < 0) { // Sem1이 0이면 대기, 아니면 -1
		perror("sem_wait");
		pthread_exit(NULL);
	}

	sprintf(Data, "This is a request from %ld.", pthread_self()); // 쓰레드 아이디 저장 

	if (sem_post(&Sem2) < 0) { // Sem2 +1;
		perror("sem_post");
		pthread_exit(NULL);
	}

	printf("Sender   -> Sent a request.....\n");

	if (sem_wait(&Sem1) < 0) { // Sem1이 0이면 대기, 아니면 -1 
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("Sender   -> Received reply: %s\n", Data); // Data 출력 

	pthread_exit(NULL); // 쓰레드 종료 
}


int main()
{
	pthread_t tid1, tid2;

	if (sem_init(&Sem1, 0, 1) < 0) { // 세마포어 생성 
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&Sem2, 0, 0) < 0) { // 세마포어 생성 
		perror("sem_init");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)Sipc1, (void *)NULL) < 0) { // 쓰레드 생성 
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&tid2, NULL, (void *)Sipc2, (void *)NULL) < 0) { // 쓰레드 생성 
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0) { // 쓰레드 종료까지 대기 
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0) { // 쓰레드 종료까지 대기 
		perror("pthread_join");
		exit(1);
	}

	if (sem_destroy(&Sem1) < 0) { // 세마포어 삭제 
		perror("sem_destroy");
		exit(1);
	}
	if (sem_destroy(&Sem2) < 0) { // 세마포어 삭제 
		perror("sem_destroy");
		exit(1);
	}

	return 0;
}

