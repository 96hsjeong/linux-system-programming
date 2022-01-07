#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// alarm 시그널 발생시 실행하는 함수 
void SigAlarmHandler(int signo)
{
	// nothing to do, just return to wake up the pause
	return;
}

// n 초간 잠자기 함수
unsigned int mysleep(unsigned int nsecs)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) { // alarm 시그널이 발생시 SigAlarmHandler 실행 
		return nsecs;
	}

	alarm(nsecs); // n초후 alarm 시그널 발생

	pause(); // 시그널 받을때까지 대기 

	return alarm(0); // alarm 시그널의 남은 초를 리턴
}

int main()
{
	printf("Wait for 5 seconds...\n");

	mysleep(5); // 5초 잠자기 실행

	return 0;
}
