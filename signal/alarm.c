#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static unsigned int AlarmSecs;

void SigAlarmHandler(int signo)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) { // alarm 시그널 받으면 SigAlarmHandler 실행 (재귀)
		perror("signal");
		exit(1);
	}

	alarm(AlarmSecs); // n초후 alarm 시그널 보냄 

	// Do something
	printf(".");
	fflush(stdout); // 출력 스트림 버퍼 비우기 

	return;
}

int SetPeriodicAlarm(unsigned int nsecs)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) { // alarm 시그널 받으면 SigAlarmHandler 실행 
		return -1;
	}

	AlarmSecs = nsecs; // 전역변수에 nsecs 저장 

	alarm(nsecs); // n초후 alarm 시그널 보냄 

	return 0;
}

int main()
{
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1); // 1초마다 . 출력

	for ( ; ; )
		pause(); // 시그널 받을때까지 대기 

	return 0;
}
