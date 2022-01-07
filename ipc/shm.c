#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE (SHM_R | SHM_W)

char Array[ARRAY_SIZE];

int main() 
{
	int shmid;
	char *ptr, *shmptr;

	if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL) { // 메모리 할당 
		perror("malloc");
		exit(1);
	}

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) { // 프로세스 종료되면 할당되 메모리도 자동으로 반납됨
		perror("shmget");
		exit(1);
	}

	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1) { // Shared Memory Attach
		perror("shmat");
		exit(1);
	}

	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]); // Data segment
	printf("Stack around %p\n", &shmid); // Stack
	printf("Malloced from %p to %p\n", ptr, ptr+MALLOC_SIZE); // Heap
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE); // Shared memory: Stack과 Heap의 중간 

	if (shmdt(shmptr) < 0) {
		perror("shmdt");
		exit(1);
	}

	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}

	return 0;
}
