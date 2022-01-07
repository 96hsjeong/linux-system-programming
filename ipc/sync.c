#include <stdio.h>
#include <stdlib.h>
#include "synclib.h"

#define NLOOPS 5

int main()
{
	int i;
	pid_t pid;

	TELL_WAIT(); // 파이프 생성 
	if ((pid = fork()) < 0) { // 프로세스 생성 
		perror("fork");
		exit(1);
	}
	else if (pid > 0) { // Parent Process
		for (i = 0; i < NLOOPS; i++) {
			TELL_CHILD(); // 파이프1에 데이터 쓰기
			printf("Parent: Tell to child\n"); 
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD(); // 파이프2에서 데이터 읽기 (읽을게 없으면 대기상태로 전환)
		}
	}
	else { // Child Process 
		for (i = 0; i < NLOOPS; i++) {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT(); // 파이프1에서 데이터 읽기 (읽을게 없으면 대기상태로 전환)
			TELL_PARENT(); // 파이프2에 데이터 쓰기
			printf("Child: Tell to parent\n");
		}
	}

	return 0;
}
