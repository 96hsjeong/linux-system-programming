#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void CharAtaTime(char *str)
{
	char *ptr;
	int c, i;

	setbuf(stdout, NULL);
	for (ptr = str; c = *ptr++;) {
		for (i = 0; i < 999999; i++); // 지연시킴
		putc(c, stdout); // 문자 하나씩 출력 
	}
}

int main()
{
	pid_t pid;

	if ((pid = fork()) < 0) { // 프로세스 생성 
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {
		CharAtaTime("output from child\n");
	}
	else {
		wait(NULL); // child 프로세스가 끝나기를 기다렸다가 실행
		CharAtaTime("output from parent\n");
	}

	return 0;
}
