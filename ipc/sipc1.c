#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"

int main()
{
	int shmid;
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

	pInt = (int *) ptr;
	while ((*pInt) == 0); // 공유메모리에 데이터 쓰여질때까지 반복 

	pData = ptr + sizeof(int);
	printf("Received request: %s......", pData);
	sprintf(pData, "This is a reply from %d.", getpid()); // 공유 메모리에 데이터 쓰기 
	*pInt = 0;
	printf("Replied.\n");

	sleep(1); // 1초 쉬기 

	if (shmctl(shmid, IPC_RMID, 0) < 0) { // 공유 메모리 삭제 
		perror("shmctl");
		exit(1);
	}

	return 0;
}
