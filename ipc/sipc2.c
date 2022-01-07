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
	pData = ptr + sizeof(int);
	sprintf(pData, "This is a request from %d.", getpid()); // 공유 메모리에 데이터 쓰기 
	*pInt = 1;
	printf("Sent a request......");

	while ((*pInt) == 1);

	printf("Received reply: %s\n", pData);

	return 0;
}
