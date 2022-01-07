#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

int main()
{
	BoundedBufferType *pBuf;
	int shmid, i, data;
	int emptySemid, fullSemid, mutexSemid;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // 공유 메모리 생성 
		perror("shmget");
		exit(1);
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1) { // 공유 메모리 연결 
		perror("shmat");
		exit(1);
	}

	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0) { // 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0) { // 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0) { // 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if (semInitValue(emptySemid, MAX_BUF) < 0) { // empty 세마포어 값을 MAX_BUF로 초기화   
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(fullSemid, 0) < 0) { // full 세마포어 값을 0으로 초기화 
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(mutexSemid, 1) < 0) { // mutex 세마포어 값으 1로 초기화 
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	srand(0x9999); // seed 값 생성 
	for (i = 0; i < NLOOPS; i++) { // NLOOPS번 만큼 반복 
		if (semWait(fullSemid) < 0) { // 비어있으면 대기 
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		if (semWait(mutexSemid) < 0) { // 다른 프로세스에서 실행 중이면 대기 
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data; // 버퍼에서 데이터 꺼내기 
		pBuf->out = (pBuf->out + 1) % MAX_BUF; // 다음에 데이터 꺼낼 인덱스 
		pBuf->counter--; // 카운터 감소 

		if (semPost(mutexSemid) < 0) { // 작업 끝나면 1 
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		if (semPost(emptySemid) < 0) { // emtpy Semaphore  +1
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		usleep((rand()%100) * 10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);

	return 0;
}

