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

	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0) { // 버퍼에 비어있는 수 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0) { // 버퍼에 들어있는 데이터 수 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0) { // Mutual Exlusion 세마포어 생성 
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	
	srand(0x8888); // Seed 값 생성 
	for (i = 0; i < NLOOPS; i++) { // NLOOPS번 만큼 반복 
		if (semWait(emptySemid) < 0) { // 버퍼가 꽉차있으면 대기 
			fprintf(stderr, "semWait failure\n"); 
			exit(1);
		}
		if (semWait(mutexSemid) < 0) { // 다른 프로세스에서 실행 중이면 대기 
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100) * 10000; // 임의의 데이터 생성 
		pBuf->buf[pBuf->in].data = data; // 버퍼에 데이터 넣기 
		pBuf->in = (pBuf->in + 1) % MAX_BUF; // 다음에 데이터 넣을 인덱스 
		pBuf->counter++; // 카운터 증가 

		if (semPost(mutexSemid) < 0) { // mutexSem Signal +1, 작업 다 끝나면 1 
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		if (semPost(fullSemid) < 0) { // fullSem Signal +1 
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		
		usleep(data);
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter);

	if (semDestroy(emptySemid) < 0) { // 세마포아 삭제 
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(fullSemid) < 0) { // 세마포어 삭제 
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0) { // 세마포어 삭제 
		fprintf(stderr, "semDestroy failure\n");
	}
	if (shmctl(shmid, IPC_RMID, 0) < 0) { // 공유 메모리 삭제 
		perror("shmctl");
		exit(1);
	}

	return 0;
}
