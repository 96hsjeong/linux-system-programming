#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// SIGUSR 시그널이 발생하면 실행하는 함수
void SigUsrHandler(int signo)
{
	if (signo == SIGUSR1) { // SIGUSR1 시그널을 받으면 실행
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2) { // SIGUSR2 시그널을 받으면 실행 
		printf("Received a SIGUSR2 signal\n");
	}
	else { // SIGUSR1, SIGUSR2 이외의 시그널을 받으면 실행 
		printf("Received unknown siganl\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

int main()
{
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR) { // 사용자 정의 시그널, kill을 통해 시그널 받으면 SigUsrHandler 실행
		perror("signal");
		exit(1);
	}
	
	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR) { // 사용자 정의 시그널, kill을 통해 시그널 받으면 SigUsrHandler 실행 
		perror("signal");
		exit(1);
	}

	for ( ; ; )
		pause(); // 시그널을 받을때까지 대기 

	return 0;
}
