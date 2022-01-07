#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
#include "synclib.h"

#define NLOOPS 10

// 포인터값 증가시키는 함수 
int update(long *ptr)
{
	return ((*ptr)++);
}

int main()
{
	int shmid, i, counter;
	char *ptr;
	pid_t pid;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // 공유 메모리 생성 
		perror("shmget");
		exit(1);
	}

	TELL_WAIT(); // 파이프 생성 
	if ((pid = fork()) < 0) { // 프로세스 생성 
		perror("fork");
		exit(1);
	}
	else if (pid > 0) { // 부모 프로세스
		if ((ptr = shmat(shmid, 0, 0)) == (void *) -1) { // 공유 메모리 붙이기 
			perror("shmget");
			exit(1);
		}
		for (i = 0; i < NLOOPS; i+= 2) { // i값 0 부터 2씩 증가하면서 반복 
			if ((counter = update((long *)ptr)) != i) { // ptr update, counter 홀수이면 에러출력 
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD(); // 자식프로세스에게 전송
			WAIT_CHILD(); // 자식프로세스 기다리기 
		}
	}
	else { // 자식 프로세스 
		if ((ptr = shmat(shmid, 0, 0)) == (void *) -1) { // 공유 메모리 붙이기 
			perror("shmget");
			exit(1);
		}
		for (i = 1; i < NLOOPS; i += 2) { // i값 1부터 2씩 증가하면서 반복 
			WAIT_PARENT(); // 부모프로세스 기다리기 
			if ((counter = update((long *)ptr)) != i) { // ptr update, counter 짝수이면 에러출력 
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT(); // 부모프로세스에게 전송 
		}
	}
	
	if (shmctl(shmid, IPC_RMID, 0) < 0) { // 공유 메모리 삭제 
		perror("shmctl");
		exit(1);
	}


	return 0;
}
