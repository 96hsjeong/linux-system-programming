// 숙제2: 추가 프로그램 작성 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
#include "semlib.h"

int main()
{
	int shmid, semid1, semid2;
	char *ptr, *pData;
	int *pInt;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // 공유 메모리 생성  
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1) { // 공유 메모리 붙이기 
		perror("shmat");
		exit(1);
	}

	if ((semid1  = semInit(SEM_KEY1)) < 0) { // 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((semid2  = semInit(SEM_KEY2)) < 0) { // 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if (semWait(semid1) < 0) { // semid1 -1, 0이면 대기 
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	
	pData = ptr;
	sprintf(pData, "This is a request from %d.", getpid()); // 공유 메모리에 데이터 쓰기 

	if (semPost(semid2) < 0) { // semid2 +1
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	printf("Sent a request......");
	
	if (semWait(semid1) < 0) { // semid1 -1, 0이면 대기  
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	printf("Received reply: %s\n", pData);

	return 0;
}
