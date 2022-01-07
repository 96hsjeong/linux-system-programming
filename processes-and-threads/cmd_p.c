#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CMD 256

// 입력 커맨드를 출력하는 함수 
void DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

int main()
{
	char cmd[MAX_CMD];
	int pid;

	while (1) {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);
		if (cmd[0] == 'q')
			break;

		if ((pid = fork()) < 0) { // 프로세스 생성 
			perror("fork");
			exit(1);
		}
		else if (pid == 0) { // child 프로세스에서 함수 실행 
			DoCmd(cmd);
			exit(0);
		}
#if 0
		else {
			wait(NULL);
		}
#endif
	}
			
	return 0;
}
