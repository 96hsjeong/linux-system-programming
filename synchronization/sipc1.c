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

	if ((semid1 = semInit(SEM_KEY1)) < 0) { // 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((semid2 = semInit(SEM_KEY2)) < 0) { // 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if (semInitValue(semid1, 1) < 0) { // 세마포어 1로 초기화 
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(semid2, 0) < 0) { // 세마포어 0으로 초기화 
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	if (semWait(semid2) < 0) { // semid2가 -1, 0이면 대기  
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	pData = ptr;
	printf("Received request: %s......", pData);
	sprintf(pData, "This is a reply from %d.", getpid()); // 공유 메모리에 데이터 쓰기 

	if (semPost(semid1) < 0) { // semid1 +1 
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	printf("Replied.\n");
	sleep(1); // 1초 쉬기 

	if (semDestroy(semid1) < 0) { // 세마포어 삭제 
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(semid2) < 0) { // 세마포어 삭제 
		fprintf(stderr, "semDestroy failure\n");
	}

	if (shmctl(shmid, IPC_RMID, 0) < 0) { // 공유 메모리 삭제 
		perror("shmctl");
		exit(1);
	}

	return 0;
}
