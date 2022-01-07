#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

int main()
{
	BoundedBufferType *pBuf;
	int shmid, i, data;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // 공유 메모리 생성 
		perror("shmget");
		exit(1);
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1) { // 공유 메모리 연결 
		perror("shmat");
		exit(1);
	}

	srand(0x8888); // Seed 값 생성 
	for (i = 0; i < NLOOPS; i++) { // NLOOPS번 만큼 반복 
		if (pBuf->counter == MAX_BUF) { // 버퍼가 꽉차있으면 
			printf("Producer: Buffer full. Waiting.....\n");
			while (pBuf->counter == MAX_BUF); // Busy Waiting 
		}

		printf("Producer: Producing an item.....\n"); 
		data = (rand()%100) * 10000; // 임의의 데이터 생성 
		pBuf->buf[pBuf->in].data = data; // 버퍼에 데이터 넣기 
		pBuf->in = (pBuf->in + 1) % MAX_BUF; // 다음에 데이터 넣을 인덱스 
		pBuf->counter++; // 카운터 증가 

		usleep(data);
	}

	printf("Producer: Producer: Produced %d items.....\n", i);

	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter);

	if (shmctl(shmid, IPC_RMID, 0) < 0) { // 공유 메모리 삭제 
		perror("shmctl");
		exit(1);
	}

	return 0;
}
