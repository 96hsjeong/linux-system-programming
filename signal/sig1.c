#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// SIGINT 신호가 오면 실행하는 함수
void SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
}

int main()
{
	signal(SIGINT, SigIntHandler); // Cntl+C 입력하여 SIGINT 신호를 보내면 SigIntHandler 실행

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause(); // 시그널을 받을때까지 대기
	
	return 0;
}
