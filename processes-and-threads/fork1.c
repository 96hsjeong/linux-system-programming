#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() 
{
	int pid;

	if ((pid = fork()) < 0) { // 프로세스 생성
		perror("fork");
		exit(1);
	}
	else if (pid == 0) { // child 프로세스는 0 반환
		// child
		printf("I am %d. My parent is %d.\n", getpid(), getppid());
	}
	else { // parent 프로세스는 child 프로세스의 pid 반환
		// parent
		printf("I am %d. My child is %d.\n", getpid(), pid);
	}

	return 0;
}

