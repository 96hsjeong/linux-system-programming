#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <string.h>

void MyAlarmHandler(int signo) // 1초 마다 함수 실행 
{
	struct passwd *rootptr;

	signal(SIGALRM, MyAlarmHandler); // alarm 시그널 받으면 다시 함수 실행 
	alarm(1);

	printf("in signal handler\n");

	if ((rootptr = getpwnam("root")) == NULL) { // root passwd 읽어옴, getpwnam: non-reentrant function
		perror("getpwnam");
		exit(1);
	}

	return;
}

int main()
{
	struct passwd *ptr;

	signal(SIGALRM, MyAlarmHandler); // alarm 시그널 받으면 함수 실행 
	alarm(1); // 1초후 alarm 시그널 발생 

	for ( ; ; ) { // 문제 없으면 계속 반복 
		if ((ptr = getpwnam("lsp41")) == NULL) { // user passwd 읽어옴, getpwnam: non-reentrant function 
			perror("getpwnam");
			exit(1);
		}

		if (strcmp(ptr->pw_name, "lsp41") != 0) { // username과 "lsp41" 비교 
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}

	return 0;
}
