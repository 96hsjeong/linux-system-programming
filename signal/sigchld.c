#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int runchild = 1; // child process 상태를 알려주는 전역변수 

// Child Process가 끝나면 함수 실행 
void SigChildHandler(int signo)
{
	printf("Child Process terminate \n");
	runchild = 0; // child process 끝났으므로 0

	return;
}

int main() 
{
	pid_t pid;
	int status;

	if (signal(SIGCHLD, SigChildHandler) == SIG_ERR) { // Signal 생성 (SIGCHLD:child 프로세스가 끝나면 신호보냄)
		perror("signal");
		exit(1);
	}

	if ((pid = fork()) < 0) { // 프로세스 생성 
		perror("fork");
		exit(1);
	}
	else if (pid == 0) { // Child Process
		printf("I'm a child\n"); 
		sleep(2); // 2sec sleep
	}
	else { // Parent Process 
		while (runchild); // child가 끝날때까지 반복 
		/** while (1) { */
			/** pause(); */
		/** } */

		printf("A child killed with %d status\n", status);
	}

	return 0;
}
