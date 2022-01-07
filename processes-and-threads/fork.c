#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int Var = 6;
char Buf[] = "a write to stdout\n";

int main()
{
	int var;
	pid_t pid;

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf)-1); // printf 와 같음
	printf("Before fork\n");

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {
		// child
		Var++; // child 프로세스에서 전역변수 Var 1 증가
		var++; // child 프로세스에서 지역변수 var 1 증가
	}
	else {
		// parent
		sleep(2); // parent 프로세스 2sec sleep
	}

	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var); 
	// child 프로세스인 경우 Var, var가 1 증가하지만, parent 프로세스는 변수값에 변화가 없음.

	return 0;
}
