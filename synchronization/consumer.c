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
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *)-1) { // 공유 메모리 연결 
		perror("shmat");
		exit(1);
	}

	srand(0x9999); // Seed 값 생성 
	for (i = 0; i < NLOOPS; i++) { // NLOOPS번 만큼 반복 
		if (pBuf->counter == 0) { // 버퍼가 비어있으면 
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (pBuf->counter == 0); // Busy Waiting
		}

		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data; // 버퍼에서 데이터 꺼내기 
		pBuf->out = (pBuf->out + 1) % MAX_BUF; // 다음에 데이터 꺼낼 인덱스 
		pBuf->counter--; // 카운터 감소 

		usleep((rand()%100) * 10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);

	return 0;
}
