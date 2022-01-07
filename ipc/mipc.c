#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"

#define NLOOPS 10
#define SIZE sizeof(long)

// 포인터가 가리키는 값을 증가시키는 함수 
int update(long *ptr) 
{
	return ((*ptr)++);
}

int main()
{
	int fd, i, counter;
	pid_t pid;
	caddr_t area;

	if ((fd = open("/dev/zero", O_RDWR)) < 0) { // 파일 읽기/쓰지 전용으로 열기 
		perror("open");
		exit(1);
	}
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (caddr_t) - 1) { // 메모리 맵핑 
		perror("mmap");
		exit(1);
	}
	close(fd); // 파일 닫기 

	TELL_WAIT();
	if ((pid = fork()) < 0) { // 프로세스 생성 
		perror("fork");
		exit(1);
	}
	else if (pid > 0) {
		for (i = 0; i < NLOOPS; i += 2) { // 0 부터 2씩 증가하며 반복 
			if ((counter = update((long *)area)) != i) { // counter update, 홀수이면 에러 
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD(); // 자식 프로세스에게 알리기 
			WAIT_CHILD(); // 자식 프로세스 기다리기 
		}
	}
	else {
		for (i = 1; i < NLOOPS; i+= 2) { // 1 부터 2씩 증가하며 반복 
			WAIT_PARENT(); // 부모 프로세스 기다리기 
			if ((counter = update((long *)area)) != i) { // counter update, 짝수이면 에러 
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT(); // 부모 프로세스에게 알리기 
		}
	}

	return 0;
}

