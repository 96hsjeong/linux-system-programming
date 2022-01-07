#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void PrintMsg(char *msg)
{
	printf("%s", msg);
	
	pthread_exit(NULL); // 쓰레드 종료
}

int main()
{
	pthread_t tid1, tid2;
	char *msg1 = "Hello, ";
	char *msg2 = "World!\n";

	if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0) { // 쓰레드 생성
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0) { // 쓰레드 생성 
		perror("pthread_create");
		exit(1);
	}

	printf("Threads created: tid=%ld, %ld\n", tid1, tid2); // thread id 출력 

	if (pthread_join(tid1, NULL) < 0) { // 쓰레드 종료 대기  
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0) { // 쓰레드 종료 대기 
		perror("pthread_join");
		exit(1);
	}

	printf("Threads terminated: tid=%ld, %ld\n", tid1, tid2); 

	return 0;
}
